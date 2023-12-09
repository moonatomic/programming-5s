#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include "headers.h"

#define THREAD_AMOUNT 20

volatile int in; // Количество потоков, которые завершили свою работу
volatile int out;

pthread_mutex_t mutex;
sem_t ready;
sem_t go;

struct thr { // Структура с данными для потока
    int n; // Размер матрицы
    double *a; // Матрица
    double *x; // Обратная матрица
    int id; // id потока
};

double *inputMatrix(FILE* fin, int n) {

    double* a = (double*)malloc(sizeof(double) * n*n);
    double number;


    for (int i = 0; i < n*n; i++) {
        if (fscanf(fin, "%lf", &number) != 1) {
            printf("Error while reading a matrix from file.\n");
            exit(1);
            return 0;
        }
        a[i] = number;
    }

    return a;
}

double solutionError(int n, double *a, double *x) {
    double ans;
    double elem;

    ans = 0.0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            elem = 0.0;
            for (int k = 0; k < n; k++) {
                elem += a[i*n + k] * x[k*n + j];
            }
            if (i == j) {
                elem -= 1;
            }
            ans += elem * elem;
        }
    }
    return sqrt(ans);
}

int invertMatrix(int n, double *a, double *x) {
    int res; // Проверка успешного создания потока

    pthread_t threads[THREAD_AMOUNT]; // Потоки
    struct thr threadt[THREAD_AMOUNT]; // Структуры с данными для потоков

    for (int i = 0; i < THREAD_AMOUNT; i++) { // Формирование данных для потоков
        threadt[i].n = n;
        threadt[i].a = a;
        threadt[i].x = x;
        threadt[i].id = i;
    }

    pthread_mutex_init(&mutex, NULL);
    sem_init(&ready, 0, 0);
    sem_init(&go, 0, 0);

    for (int i = 0; i < THREAD_AMOUNT; i++) { // Создаем потоки
        res = pthread_create(&threads[i], NULL, f_thr, &threadt[i]);
        if (res != 0) {
            printf("Error: unable to create a thread.\n");
            return -1;
        }
    }

    for (int i = 0; i < THREAD_AMOUNT; i++) { // По окончании работы всех потоков джойним их
        res = pthread_join(threads[i], NULL);
        if (res != 0) {
            printf("Error: unable to join a thread.\n");
            return -1;
        }
    }

    return 0;
}

void printMatrix(int n, int m, double *a) {
    int b = (n > m ? m : n);
    for (int i = 0; i < b; i++) {
        for (int j = 0; j < b; j++) {
            printf("%10.3g ", a[i * n + j]);
        }
        printf("\n");
    }
}

void *f_thr(void *arg) {
    int leader; // Переменные для алгоритма
    double swp;
    double coeff;
    double norm;
    double max;

    int i_f; // Номер первой строки блока для потока
    int i_l; // Номер последней строки блока

    struct thr *inp;
    int n;
    double *a;
    double *x;
    int id;
    inp = (struct thr *)arg;
    n = inp->n; // Распаковка структуры с данными
    a = inp->a;
    x = inp->x;
    id = inp->id;

    i_f = id * (n/THREAD_AMOUNT); // Первый индекс у всех потоков вычисляется одинаково
    if (id < THREAD_AMOUNT - 1) { // Последний индекс для посленего потока ограничивается размером матрицы
        i_l = (id+1) * (n/THREAD_AMOUNT);
    } else {
        i_l = n;
    }

    for (int i = 0; i < n; i++) {
        // printf("Thread %d started at i = %d\n", id, i);
        if (id == 0) { // Нулевой поток ищет лидера, переставляет его наверх и нормирует строку
            max = a[i*n + i];
            leader = i;
            for (int j = i+1; j < n; j++) { //Ищем лидера по столбцу
                if (max < fabs(a[j*n + i])) {
                    max = fabs(a[j*n + i]);
                    leader = j;
                }
            }
            if (i != leader) { // Переставляем строку с лидером наверх
                for (int j = 0; j < n; j++) {
                    swp = a[i*n + j];
                    a[i*n + j] = a[leader*n + j];
                    a[leader*n + j] = swp;
                } // То же самое во второй матрице
                for (int j = 0; j < n; j++) {
                    swp = x[i*n + j];
                    x[i*n + j] = x[leader*n + j];
                    x[leader*n + j] = swp;
                }
            }
            
            norm = 1.0 / a[i*n + i];
            for (int j = 0; j < n; j++) { // Нормируем верхнюю строку
                a[i*n + j] *= norm;
            }
            for (int j = 0; j < n; j++) {
                x[i*n + j] *= norm;
            }
            // printf("Thread 0 finished leader search and placed it to (%d,%d)\n", i, i);
        }

        

        pthread_mutex_lock(&mutex); // Каждый поток заканчивает работу и ждет других
        in++;
        // printf("Thread %d increased in at %p to %d\n", id, &in, in);
        if (in == THREAD_AMOUNT) { // Если поток закончил работу последним, он пропускает всех остальных
            for (int p = 0; p < THREAD_AMOUNT; p++) {
                sem_post(&go);
            }
            in = 0; // И обнуляет количество для следующей итерации
        }
        pthread_mutex_unlock(&mutex);
        sem_wait(&go);

        for (int j = i_f; j < i_l; j++) { // Вычитает строку из всех остальных в своем блоке
            if (j != i) {
                coeff = a[j*n + i];
                for (int k = i; k < n; k++) {
                    a[j*n + k] -= a[i*n + k] * coeff;
                }

                for (int k = 0; k < n; k++) {
                    x[j*n + k] -= x[i*n + k] * coeff;
                }
            }
        }

        // printf("Thread %d finished on lines %d-%d\n", id, i_f, i_l);

        pthread_mutex_lock(&mutex); // Каждый поток заканчивает работу и ждет других
        out++;
        if (out == THREAD_AMOUNT) { // Если поток закончил работу последним, он пропускает всех остальных
            for (int p = 0; p < THREAD_AMOUNT; p++) {
                sem_post(&ready);
            }
            out = 0; // И обнуляет количество для следующей итерации
        }
        pthread_mutex_unlock(&mutex);
        sem_wait(&ready);
        // printf("Thread %d finished at i = %d\n", id, i);
    }
}
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>

#define WRITERS 2 // Количество писателей
#define READERS 4 // Количество читателей
#define DURATION 30 // Длительность работы

sem_t s_writers; // Семафоры-двери библиотеки
sem_t s_readers;
pthread_mutex_t mutex; // Мьютекс для общих данных

int status;
int readers;
int writers;

void *f_reader(void *arg); // Рутины читателей и писателей
void *f_writer(void *arg);

int main(void) {
    srand(time(NULL));

    pthread_t t_writers[WRITERS]; // Потоки читатели и писатели
    pthread_t t_readers[READERS];

    status = 0;

    sem_init(&s_readers, 0, READERS); // Инициализация семафоров
    sem_init(&s_writers, 0 , 1);
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < WRITERS; i++) { // Создание потоков писателей
        if (pthread_create(&t_writers[i], NULL, f_writer, 0) != 0) {
            printf("Error: unable to create a thread for a writer.\n");
            return -1;
        }
    }

    for (int i = 0; i < READERS; i++) { // Создание потоков читателей
        if (pthread_create(&t_readers[i], NULL, f_reader, 0) != 0) {
            printf("Error: unable to create a thread for a reader.\n");
            return -1;
        }
    }
    sleep(DURATION); // Библиотека работает
    pthread_mutex_lock(&mutex);
    printf("Readers visited: %d\nWriters visited: %d\n", readers, writers);
    pthread_mutex_unlock(&mutex);

    return 0;
}

void *f_reader(void *arg) {
    int waiting;
    while (1) { // Каждый такт читатель решает, заходить ли ему в библиотеку
        if (rand()%2 == 0) {
            sleep(1);
        } else {
            waiting = 0;
            printf("A reader is waiting\n");
            while (1) { // Читатель может входить только когда в библиотеке нет писателей
                pthread_mutex_lock(&mutex); // Проверяет, можно ли входить
                if (status >= 0) { // Если все писатели вышли
                    status++; // Объявляет, что он заходит в библиотеку
                    pthread_mutex_unlock(&mutex);
                    break;
                } else { // Если в библиотеке есть писатель, закрывает дверь и ждет, пока кто-то выйдет
                    waiting = 1;
                    pthread_mutex_unlock(&mutex);
                    sem_wait(&s_readers);
                }
            }
            if (waiting == 1) {
                sem_post(&s_readers);
            }
            printf("A reader is reading\n");
            sleep(1); // Читает
            pthread_mutex_lock(&mutex); // Уменьшает количество читателей и уходит из библиотеки
            status--;
            readers++;
            pthread_mutex_unlock(&mutex);
            printf("A reader goes away\n");
            sem_post(&s_readers); // Открывает двери всем
            sem_post(&s_writers);
        }
        sleep(1);
    }
}

void *f_writer(void *arg) {
    while (1) { // Каждый такт писатель решает, заходить ли ему в библиотеку
        if (rand()%2 == 0) {
            sleep(1);
        } else {
            printf("A writer is waiting\n");
            while (1) { // Писатель может входить только когда в библиотеке нет никого, поэтому он ждет
                pthread_mutex_lock(&mutex); // Проверяет, можно ли входить
                if (status == 0) { // Если все вышли
                    status--; // Объявляет, что он заходит в библиотеку
                    pthread_mutex_unlock(&mutex);
                    break;
                } else { // Если в библиотеке кто-то есть, закрывает дверь и ждет, пока кто-то выйдет
                    pthread_mutex_unlock(&mutex);
                    sem_wait(&s_writers);
                }
            }
            printf("A writer is writing\n");
            sleep(1); // Пишет
            pthread_mutex_lock(&mutex); // Уменьшает количество писателей и уходит из библиотеки
            status++;
            writers++;
            pthread_mutex_unlock(&mutex);
            printf("A writer goes away\n");
            sem_post(&s_readers); // Открывает двери всем
            sem_post(&s_writers);
        }
    }
}

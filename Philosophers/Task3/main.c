#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define LUNCHTIME 60 // Длительность обеда

pthread_mutex_t m_forks[5]; // Мьютексы-вилки
sem_t s_th;                 // Семафор для поочередного создания потоков
int counter[5];             // Счетчик приемов пищи

void *f_philosophers(void *arg); // Рутина философа

int main(void) {
    int ins; // Проверка удачного создания потока

    pthread_t t_philosophers[5]; // Потоки-философы

    for (int i = 0; i < 5; i++) { // Инициализация массива результатов
        counter[i] = 0;
    }
    
    sem_init(&s_th, 0, 0);
    for (int i = 0; i < 5; i++) {
        pthread_mutex_init(&m_forks[i], NULL); // Инциализация семафора и мьютексов
    }

    for (int i = 0; i < 5; i++) { // Создание потоков-философов
        ins = pthread_create(&t_philosophers[i], NULL, f_philosophers, &i);
        if (ins != 0) {
            printf("Error: unable to create a thread for a philosopher.\n");
            exit(1);
        }
        printf("Philosopher %d sat at the table.\n", i+1);
        sem_wait(&s_th);
    }

    sleep(LUNCHTIME); // Условие выхода - обед закончился

    for (int i = 0; i < 5; i++) { // Вывод результатов
        printf("Philosopher %d ate %d times.\n", i+1, counter[i]);
    }

    return 0;
}

void *f_philosophers(void *arg) {
    int num = *(int *)arg;
    int f_fork;
    int s_fork;
    if (num%2 == 1) { // В зависимости от четности распределяем вилки
        f_fork = num; // Первая вилка у нечетного философа - левая
        s_fork = (num+1) % 5;
    } else {
        s_fork = num; // У четного - правая
        f_fork = (num+1) % 5;
    }
    sem_post(&s_th); // Поток создан

    while (1) {
        printf("Philosopher %d is reflecting.\n", num+1); // Философ размышляет в течение 1 секунды
        sleep(1);
        pthread_mutex_lock(&m_forks[f_fork]); // Философ берет первую вилку
        printf("Philosopher %d took the first fork.\n", num+1);
        pthread_mutex_lock(&m_forks[s_fork]); // Философ берет вторую вилку
        printf("Philosopher %d took the second fork and is eating.\n", num+1);
        sleep(2);       // Ест в течение 2 секунд
        counter[num]++; // Увеличиваем его результат
        pthread_mutex_unlock(&m_forks[s_fork]); // Философ кладет вторую вилку
        printf("Philosopher %d put the second fork down.\n", num+1);
        pthread_mutex_unlock(&m_forks[f_fork]); // Философ кладет первую вилку
        printf("Philosopher %d put the first fork down.\n", num+1);
    }
}

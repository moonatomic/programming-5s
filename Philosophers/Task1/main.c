#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define LUNCHTIME 60 // Длительность обеда
#define ATTEMPTS 2   // Количество попыток взять вилку

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
    int l_fork = num;         // Левая вилка
    int r_fork = (num+1) % 5; // Правая вилка
    sem_post(&s_th);          // Поток создан

    while (1) {
        printf("Philosopher %d is reflecting.\n", num+1); // Философ размышляет в течение 1 секунды
        sleep(1);
        pthread_mutex_lock(&m_forks[l_fork]); // Философ берет левую вилку
        printf("Philosopher %d took the left fork.\n", num+1);
        for (int i = 0; i < ATTEMPTS; i++) { // Делает ATTEMPTS попыток взять правую вилку
            if (pthread_mutex_trylock(&m_forks[r_fork]) == 0) {
                printf("Philosopher %d took the right fork and is eating.\n", num+1);
                sleep(2);       // Если удалось, ест в течение 2 секунд
                counter[num]++; // Увеличиваем его результат
                pthread_mutex_unlock(&m_forks[r_fork]); // Философ кладет правую вилку
                printf("Philosopher %d put the right fork down.\n", num+1);
                break;
            } else {
                sleep(1); // Если не получилось, ждем
            }
        }
        pthread_mutex_unlock(&m_forks[l_fork]); // Вне зависимости от того, поел философ или нет,
        printf("Philosopher %d put the left fork down.\n", num+1); // кладем левую вилку
    }
}

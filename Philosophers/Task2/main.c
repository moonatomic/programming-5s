#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define LUNCHTIME 90 // Длительность обеда
#define MAXHUNGER 20 // Величина голода, при которой обед завершается


pthread_mutex_t m_forks[5]; // Мьютексы-вилки
sem_t s_th;                 // Семафор для поочередного создания потоков
sem_t s_glutton;            // Семафор для обжор 
int counter[5];             // Счетчик приемов пищи

void *f_philosophers(void *arg); // Рутина философа
void *f_gluttons(void *arg);     // Рутина обжоры

int main(void) {
    int ins; // Проверка удачного создания потока

    pthread_t t_philosophers[5]; // Потоки-философы

    for (int i = 0; i < 5; i++) { // Инициализация массива результатов
        counter[i] = 0;
    }
    
    sem_init(&s_th, 0, 0); // Инициализация семафоров
    sem_init(&s_glutton, 0, 2);

    for (int i = 0; i < 5; i++) {
        pthread_mutex_init(&m_forks[i], NULL); // Инциализация мьютексов
    }

    for (int i = 0; i < 5; i++) { // Создание потоков-философов
        if (i == 2 || i == 4) {
            ins = pthread_create(&t_philosophers[i], NULL, f_gluttons, &i); // 2 и 4 философы - обжоры
        } else {
            ins = pthread_create(&t_philosophers[i], NULL, f_philosophers, &i);
        }
        if (ins != 0) {
            printf("Error: unable to create a thread for a philosopher.\n");
            exit(1);
        }
        printf("Philosopher %d sat at the table.\n", i+1);
        sem_wait(&s_th);
    }

    sleep(LUNCHTIME); // Если выход не произошел раньше, выходим после окончания обеда

    for (int i = 0; i < 5; i++) { // Вывод результатов
        printf("Philosopher %d ate %d times.\n", i+1, counter[i]);
    }

    return 0;
}

void *f_philosophers(void *arg) {
    int num = *(int *)arg;
    int hunger = 0;           // Показатель голода
    int l_fork = num;         // Левая вилка
    int r_fork = (num+1) % 5; // Правая вилка
    sem_post(&s_th);          // Поток создан

    while (1) {
        printf("Philosopher %d is reflecting.\n", num+1); // Философ размышляет в течение 1 секунды
        sleep(1);
        pthread_mutex_lock(&m_forks[l_fork]); // Философ берет левую вилку
        printf("Philosopher %d took the left fork.\n", num+1);
        if (pthread_mutex_trylock(&m_forks[r_fork]) == 0) {
            printf("Philosopher %d took the right fork and is eating.\n", num+1);
            sleep(2);       // Если удалось, ест в течение 2 секунд
            counter[num]++; // Увеличиваем его результат
            hunger = 0;     // Обнуляем голод
            pthread_mutex_unlock(&m_forks[r_fork]); // Философ кладет правую вилку
            printf("Philosopher %d put the right fork down.\n", num+1);
        } // Если не получилось взять правую вилку
        hunger++; // Голод увеличивается
        if (hunger >= MAXHUNGER) { // Если он становится слишком высоким, сообщаем об этом
            printf("Philosopher %d was not served.\nEat count for all philosophers:\n", num+1);
            for (int i = 0; i < 5; i++) {
                printf("%d ", counter[i]); // Выводим общий результат
            }
            printf("\n");
            exit(2);
        }
        pthread_mutex_unlock(&m_forks[l_fork]); // Вне зависимости от того, получилось ли поесть,
        printf("Philosopher %d put the left fork down.\n", num+1); // кладем левую вилку
    }
}

void *f_gluttons(void *arg) {
    int num = *(int *)arg;
    int l_fork = num;         // Левая вилка
    int r_fork = (num+1) % 5; // Правая вилка
    sem_post(&s_th);          // Поток создан

    while (1) {
        pthread_mutex_lock(&m_forks[l_fork]); // Берет обе вилки
        printf("Philosopher-glutton %d took the left fork.\n", num+1);
        pthread_mutex_lock(&m_forks[r_fork]);
        printf("Philosopher-glutton %d took the right fork.\n", num+1);
        sem_post(&s_glutton);                   // Говорит второму обжоре, что можно класть вилки
        sleep(2);                               // Ест в течение 2 секунд
        counter[num]++;
        sem_wait(&s_glutton);                   // Ждет, пока второй обжора возьмет вилки
        pthread_mutex_unlock(&m_forks[l_fork]); // Кладет обе вилки
        printf("Philosopher-glutton %d put the left fork down.\n", num+1);
        pthread_mutex_unlock(&m_forks[r_fork]);
        printf("Philosopher-glutton %d put the right fork down.\n", num+1);
    }
}

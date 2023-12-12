#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define SOFA 10 // Количество мест на диване
#define CHAIRS 6 // Количество стульев
#define SHAVE_TIME 1 // Время бритья
#define DURATION 60 // Длительность рабочего дня
#define CUSTOMERS 30 // Количество клиентов

sem_t s_sofa;   // Семафор для дивана
sem_t s_chairs; // Семафор для стульев
sem_t s_exit;   // Семафор для выхода
sem_t s_shave;  // Семафор для сна брадобрея
sem_t s_th;     // Семафор для поочередного создания потоков
pthread_mutex_t m_barber; // Мьютекс-кресло брадобрея

void *f_customers(void *arg); // Объявление рутин посетителей и брадобрея
void *f_barber(void *arg);

int counter; // Счетчик обслуженных клиентов

int main(void) {
    int ins; // Проверка удачного создания потока

    counter = 0; // Инициализация счетчика

    sem_init(&s_sofa, 0, SOFA); // Инициализация семафоров и мьютекса
    sem_init(&s_chairs, 0, CHAIRS);
    sem_init(&s_exit, 0, 0);
    sem_init(&s_shave, 0, 0);
    sem_init(&s_th, 0, 0);
    pthread_mutex_init(&m_barber, NULL);

    pthread_t t_customers[CUSTOMERS]; // Потоки-клиенты
    pthread_t t_barber;               // Поток-брадобрей

    ins = pthread_create(&t_barber, NULL, f_barber, 0); // Создание потока-брадобрея
    if (ins != 0) {
            printf("Error: unable to create a thread for the barber.\n");
            exit(1);
        }

    for (int i = 0; i < CUSTOMERS; i++) { // Создание потоков-клиентов
        ins = pthread_create(&t_customers[i], NULL, f_customers, &i);
        if (ins != 0) {
            printf("Error: unable to create a thread for a cutomer.\n");
            exit(1);
        }
        sem_wait(&s_th);
    }

    sleep(DURATION);

    printf("%d customers served.\n", counter);
}

void *f_customers(void *arg) {
    int num = *(int *)arg; // Присваиваем клиенту номер
    sem_post(&s_th); // Поток создан

    sem_wait(&s_sofa); // Ожидает места на диване
    printf("Customer is waiting to come in.\n");
    sem_wait(&s_chairs); // Ожидает свободного стула
    sem_post(&s_sofa);  // Освобождает место на диване
    printf("Customer is sitting on a chair.\n");
    pthread_mutex_lock(&m_barber); // Ожидает свободного кресла брадобрея
    sem_post(&s_chairs); // Освобождает стул
    printf("Customer is in the barber's room.\n");
    sem_post(&s_shave); // Будит брадобрея
    sem_wait(&s_exit); // Ждет, пока его побреют
    printf("Customer goes away.\n");
    pthread_mutex_unlock(&m_barber); // Освобождает кресло брадобрея и уходит
}

void *f_barber(void *arg) {
    while(1) {
        sem_wait(&s_shave); // Спит, пока не придет клиент
        printf("Barber wakes up and is shaving the client.\n");
        sleep(SHAVE_TIME); // Бреет клиента
        counter++; // Увеличивает счетчик
        sem_post(&s_exit); // Отпускает клиента
        printf("Barber is sleeping.\n");
    }
}

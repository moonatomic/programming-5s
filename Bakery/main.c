#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define BAKERS_AMOUNT 3     // Количество пекарей
#define CUSTOMERS_AMOUNT 14 // Количество посетителей
#define CAPACITY 5          // Вместимость прилавка
#define BAKING_DURATION 6   // Время приготовления одной булки
#define WORKDAY 60          // Длительность рабочего дня

sem_t s_bakers, s_customers; // Семафоры для пекарей и посетителей
sem_t s_th;                  // Семафор для поочередного выделения потоков

void *f_customers(void *arg); // Объявление рутин для посетителей и пекарей
void *f_bakers(void *arg); 

int main(void) {
    int ins; // Проверка удачного создания потока

    pthread_t t_bakers[BAKERS_AMOUNT];       // Потоки-пекари
    pthread_t t_customers[CUSTOMERS_AMOUNT]; // Потоки-посетители

    sem_init(&s_bakers, 0, CAPACITY); // Инициализация семафоров для пекарей
    sem_init(&s_customers, 0, 0);     // и посетителей
    sem_init(&s_th, 0, 0);
    
    for (int i = 0; i < BAKERS_AMOUNT; i++) { // Создание потоков-пекарей
        ins = pthread_create(&t_bakers[i], NULL, f_bakers, &i);
        if (ins != 0) {
            printf("Error: unable to create a thread for a baker.\n");
            exit(1);
        }
        sem_wait(&s_th);
    }

    for (int i = 0; i < CUSTOMERS_AMOUNT; i++) { // Создание потоков-посетителей
        ins = pthread_create(&t_customers[i], NULL, f_customers, &i);
        if (ins != 0) {
            printf("Error: unable to create a thread for a customer.\n");
            exit(1);
        }
        sem_wait(&s_th);
    }

    sleep(WORKDAY); // Условие выхода - закончился рабочий день
    return 0;
}


void *f_bakers(void *arg) {
    int num = *(int*)arg;
    sem_post(&s_th); // Поток-пекарь создан
    // int space;       // Количество оставшихся мест на прилавке
    
    while (1) {
        sem_wait(&s_bakers);             // Выделяем место на прилавке
        // sem_getvalue(&s_bakers, &space); // Узнаем количество свободных мест
        sleep(BAKING_DURATION);          // Печем булку
        sem_post(&s_customers);          // Кладем булку на прилавок
        printf("Пекарь %d испек булку.\n", num+1);
    }
}

void *f_customers(void *arg) {
    int num = *(int*)arg; 
    sem_post(&s_th); // Поток-посетитель создан
    // int loafs;       // Количество оставшихся булок на прилавке
    
    while (1) {
        sem_wait(&s_customers);             // Выбираем булку на прилавке
        // sem_getvalue(&s_customers, &loafs); // Узнаем количество булок
        sleep(2);                           // Оплачиваем булку 
        sem_post(&s_bakers);                // Забираем булку с прилавка
        printf("Посетитель %d купил булку.\n", num+1);
    }
}

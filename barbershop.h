#ifndef BARBERSHOP_H
#define BARBERSHOP_H

#include <semaphore.h>
#include "fifo.h"
#include <time.h>
#include <stdio.h>

typedef struct barbershop barbershop;

struct barbershop
{
    sem_t max_capacity;
    sem_t sofa;
    sem_t barber_chair;
    sem_t coord;
    sem_t mutex1;
    sem_t mutex2;
    sem_t mutex3;
    sem_t cust_ready;
    sem_t payment;

    //Added to ensure fair lineups while waiting in line
    int count;
    int sofacount;
    int barbcount;

    //Added for easier revenue tracking
    int revenue;

    fifo queue1;
    fifo queue2;

    //Added for easier event logging
    FILE *log;

    int simStartTime;
    double realStartTime;
};

/* modify (and complete) the following functions */
void *Barber( void* arg );
void *Cashier( void* arg );

#endif

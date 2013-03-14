#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <pthread.h>
#include <semaphore.h>
#include "barbershop.h"
#include <strings.h>

typedef struct customer_config customer_config;
typedef struct customer customer;

struct customer_config
{
   char arrivalTime[9];
   char serviceTime[6];
   char payment[7];
   char type[6];
};

struct customer
{
   barbershop* bs;
   customer_config config;
   pthread_t thread;
   int id;
   int arrivalTime;
   sem_t leave_b_chair;
   sem_t receipt;
   sem_t finished;
   int stillWaiting;
};

/* modify (and complete) the following functions */
void *Customer( void* arg );
int LoadCustomerData( char* customerDataFileName, customer customers[], int numCustomers );
void PrintCustomerData( customer* c );

#endif

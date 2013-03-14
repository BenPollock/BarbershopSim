#include <semaphore.h>
#include "helpers.h"
#include "fifo.h"
#include "customer.h"

void *Customer( void* arg )
{
   customer* c = (customer*)arg;
   c->stillWaiting = 1;
   c->arrivalTime = GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime);

   char *stamp = malloc(sizeof(char)*15);

    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));

    sem_init(&c->leave_b_chair, 0, 0);
    sem_init(&c->finished, 0, 0);
    sem_init(&c->receipt, 0, 0);

    int temp = 0;
    sem_getvalue(&c->bs->max_capacity, &temp);

    //Wait for there to be room
   sem_wait(&c->bs->max_capacity);

   //Kill self if there's no more time
    if(GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime) > (3600*17)){
            TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
            printf("<%s>Customer %d is turned away.\n", stamp, c->id);
            fprintf(c->bs->log,"<%s>Customer %d is turned away.\n", stamp, c->id);
           pthread_exit(NULL);
       }


   int curCount = -1;

   TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
   printf("<%s>Customer %d arrives to the barber shop\n", stamp, c->id);
   fprintf(c->bs->log,"<%s>Customer %d arrives to the barber shop\n", stamp, c->id);


    //Enter the waiting room
    //Make sure no one else can get in while we can
    //And only go in if we are the next in line
    do{
        curCount = c->bs->count;
    }while(curCount != c->id);

    sem_wait(&c->bs->mutex1);
    c->bs->count++;
    sem_post(&c->bs->mutex1);

    curCount = -1;

    c->stillWaiting = 0;
    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
     printf("<%s>Customer %d enters the waiting room\n", stamp, c->id);
     fprintf(c->bs->log,"<%s>Customer %d enters the waiting room\n", stamp, c->id);
     fflush(stdout);

    //Wait for there to be room on the sofa and
    //for it to be our turn
    do{
        curCount = c->bs->sofacount;
    }while(curCount != c->id);
    sem_wait(&c->bs->sofa);
    c->bs->sofacount++;

    curCount = -1;

    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
    printf("<%s>Customer %d sits on sofa\n", stamp, c->id);
    fprintf(c->bs->log,"<%s>Customer %d sits on sofa\n", stamp, c->id);
    fflush(stdout);

    //Once we're on the sofa, wait for the barber chair to open up
    //and for it to be our turn
    do{
        curCount = c->bs->barbcount;
    }while(curCount != c->id);
    sem_wait(&c->bs->barber_chair);
    c->bs->barbcount++;
    sem_post(&c->bs->sofa);

    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
    printf("<%s>Customer %d goes on barber chair\n", stamp, c->id);
    fprintf(c->bs->log,"<%s>Customer %d goes on barber chair\n", stamp, c->id);

    //Wait in barber chair and add to the queue
    sem_wait(&c->bs->mutex2);
    Enqueue(&c->bs->queue1, c->id);
    sem_post(&c->bs->cust_ready);
    sem_post(&c->bs->mutex2);

    //Finish haircut
    sem_wait(&c->finished);
    sem_post(&c->leave_b_chair);

    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
    printf("<%s>Customer %d leaves barber chair\n", stamp, c->id);
    fprintf(c->bs->log,"<%s>Customer %d leaves barber chair\n", stamp, c->id);

    //Wait to pay
    sem_wait(&c->bs->mutex3);
    Enqueue(&c->bs->queue2, c->id);
    sem_post(&c->bs->payment);
    sem_post(&c->bs->mutex3);

    //Get receipt
    sem_wait(&c->receipt);

    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
     printf("<%s>Customer %d receives receipt\n", stamp, c->id);
     fprintf(c->bs->log,"<%s>Customer %d receives receipt\n", stamp, c->id);
     fflush(stdout);

    //Leave
    sem_post(&c->bs->max_capacity);
    TimeStampSec2Str(stamp, GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime));
     printf("<%s>Customer %d leaves the barber shop.\n", stamp, c->id);
    fprintf(c->bs->log,"<%s>Customer %d leaves the barber shop.\n", stamp, c->id);
     fflush(stdout);


    c->arrivalTime = GetSimTimeStampSec(c->bs->simStartTime, c->bs->realStartTime) - c->arrivalTime;

   pthread_exit(NULL);
}

/* Load customer data file and store contents in an array of customer structs. */
/* 'LoadCustomerData' returns -1 if there are complications during the load; returns 1 on success. */
int LoadCustomerData( char* customerDataFileName, customer customers[], int numCustomers )
{
	FILE *customerData;
	customerData = fopen(customerDataFileName, "rt");
	int i = 0;
	char * line = malloc(sizeof(char)*80);
    int count;
	while(fgets(line, 80, customerData) != NULL && i<= numCustomers)
	{
	    i++;
	    count++;
		char *tokens;
		tokens = strtok(line, " ");
        char *a = malloc(sizeof(char)*25);
        strcpy(a, tokens);
        tokens = strtok(NULL, " ");
        char *b = malloc(sizeof(char)*25);
        strcpy(b, tokens);
        tokens = strtok(NULL, " ");
        char *c = malloc(sizeof(char) * 25);
        strcpy(c, tokens);
        tokens = strtok(NULL, " ");
       char *d = malloc(sizeof(char) * 25);
        strcpy(d, tokens);
		struct customer_config newCust;
		strncpy(newCust.arrivalTime, a, 9);
		strncpy(newCust.serviceTime, b, 6);
		strncpy(newCust.payment, c, 7);
		strncpy(newCust.type, d, 6);
		customers[i].config = newCust;

		i++;

	}

   return 1;
}

/* Print customer data. */
void PrintCustomerData( customer* c )
{
   printf( "Arrival Time = %s, Service Time = %s, Payment = %s, Type = %s\n",
      c->config.arrivalTime, c->config.serviceTime, c->config.payment, c->config.type);
}

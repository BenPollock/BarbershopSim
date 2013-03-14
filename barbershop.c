#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

#include "barbershop.h"
#include "helpers.h"
#include "customer.h"

customer* customers;

void *Barber( void* arg )
{
    barbershop* bs = (barbershop*)arg;
      char *stamp = malloc(sizeof(char)*15);

    TimeStampSec2Str(stamp, GetSimTimeStampSec(bs->simStartTime, bs->realStartTime));

while (1){

   //Wait for a customer to be ready
   sem_wait(&bs->cust_ready);

   //Get the customer
   sem_wait(&bs->mutex2);
   int custNum = Dequeue(&bs->queue1);
   sem_post(&bs->mutex2);

   TimeStampSec2Str(stamp, GetSimTimeStampSec(bs->simStartTime, bs->realStartTime));

   //Cut hair
   sem_wait(&bs->coord);
   int custHairTime = ServiceTimeStr2Sec(customers[custNum].config.serviceTime);
    SimOperation(custHairTime);
    sem_post(&bs->coord);

    sem_post(&customers[custNum].finished);
    sem_wait(&customers[custNum].leave_b_chair);
    sem_post(&bs->barber_chair);
    fflush(stdout);

}

   //END STUFF

   pthread_exit(NULL);
}

void *Cashier( void* arg )
{
   barbershop* bs = (barbershop*)arg;

   while(1){

    //Wait for somebody to need payment
    sem_wait(&bs->payment);

    //Get the customer
    sem_wait(&bs->mutex3);
    int custNum = Dequeue(&bs->queue2);
    sem_post(&bs->mutex3);

    sem_wait(&bs->coord);
    //Get the payment, sleep
    SimOperation(GetCashierDelay(customers[custNum].config.payment));
    sem_post(&bs->coord);
    sem_post(&customers[custNum].receipt);


    char* custType = customers[custNum].config.type;
    custType[strlen(custType)-1] = 0;

    //Increase Revenue
    bs->revenue = bs->revenue + (int)GetHairCutPrice(custType);

   }


   pthread_exit(NULL);
}

int main( int argc, char *argv[] )
{
   barbershop bs;
   bs.count = 0;
   bs.sofacount = 0;
   bs.barbcount = 0;
   bs.revenue = 0;

   bs.log = fopen("barber_log.txt", "w+");


   pthread_t barberThreads[3];
   pthread_t cashierThread;


   /* prepare fifo queues */
   bs.queue1.tail = NULL;
   bs.queue1.head = NULL;
   bs.queue2.tail = NULL;
   bs.queue2.head = NULL;

   fflush(stdout);

   /* make sure barbershop program has appropriate arguments */
   if(argc != 3)
   {
      printf("usage: barbershop <NUM_CUSTOMERS> <CUSTOMER_DATA_FILE>\n");
      exit(0);
   }
   char* customerDataFileName = argv[2];
   int numCustomers = atoi(argv[1]);


   /* allocate memory to store customer data */
  // customer* customers;
   if((customers = (customer*)malloc(sizeof(customer)*numCustomers)) == NULL)
   {
      perror("Error while allocating memory for customer data");
      exit(1);
   }

   //Create customer threads
   pthread_t customerThread[numCustomers];

   /* load in customer data from file */
   if(LoadCustomerData(customerDataFileName, customers, numCustomers) == -1)
   {
      printf("Error while loading customer data file\n");
      exit(1);
   }

   //Mutexes
   sem_init(&bs.mutex1, 0, 1);
    sem_init(&bs.mutex2, 0, 1);
    sem_init(&bs.mutex3, 0, 1);


    //Customer Locations
    sem_init(&bs.max_capacity, 0, 13);
    sem_init(&bs.sofa, 0, 4);
    sem_init(&bs.barber_chair, 0, 3);
    sem_init(&bs.cust_ready, 0, 0);
    sem_init(&bs.coord, 0, 3);
    sem_init(&bs.payment, 0, 0);

    //Create the cashier thread
    pthread_create(&cashierThread, NULL, Cashier, (void*)&bs);

    //Create the barber threads
    for(int i = 0; i < 3; i++)
        pthread_create(&barberThreads[i], NULL, Barber, (void*)&bs);

    bs.simStartTime = 32400;
    bs.realStartTime = GetRealTimeStamp();

    int currentCustomer = 0;

    //Create customer threads
    while(currentCustomer < numCustomers){

        int curTime = GetSimTimeStampSec(bs.simStartTime, bs.realStartTime);
        int custTime = TimeStampStr2Sec(customers[currentCustomer].config.arrivalTime);
        char* createTime = malloc(sizeof(char)*10);
        TimeStampSec2Str(createTime, curTime);
        if(curTime > custTime && curTime < (17*3600)){
            customers[currentCustomer].arrivalTime = curTime;
            customers[currentCustomer].bs = &bs;
            customers[currentCustomer].id = currentCustomer;
            pthread_create(&customerThread[currentCustomer], NULL, Customer, (void*)&customers[currentCustomer]);
            currentCustomer++;
        }else if(curTime > (17*3600)){
            printf("It's now 5:00, no new customers will be allowed in.\n");
            break;
        }

    }

    int turnedAway = numCustomers - currentCustomer;
    int lowestTurnedAway = numCustomers + 1;
    //At this point, we need to terminate customers who are currently waiting outside
    for(int i = 0; i < currentCustomer; i++){
        if(customers[i].stillWaiting == 1){
            if(i < lowestTurnedAway)
                lowestTurnedAway = i;
            turnedAway++;
        }
    }
    if(turnedAway == 0)
        lowestTurnedAway = numCustomers;

    //WAIT FOR THREADS TO FINISH BEFORE TERMINATING

    sleep(5);

    //Close Semaphores
    sem_close(&bs.mutex1);
    sem_close(&bs.mutex2);
    sem_close(&bs.mutex3);
    sem_close(&bs.max_capacity);
    sem_close(&bs.sofa);
    sem_close(&bs.barber_chair);
    sem_close(&bs.cust_ready);
    sem_close(&bs.coord);
    sem_close(&bs.payment);

    //Get Total/Avg Time
    int avgHaircut = 0;
    int avgStay = 0;

    for(int i = 0; i < lowestTurnedAway; i++){
        avgHaircut = avgHaircut + ServiceTimeStr2Sec(customers[i].config.serviceTime);
        avgStay = avgStay + customers[i].arrivalTime;

    }
    avgStay = (int)(avgStay / lowestTurnedAway);


    //Statistics
    printf("Revenue is %d\n", bs.revenue);
    printf("%d Customers have been turned away\n", turnedAway);
    printf("Total customers served is %d\n", numCustomers - turnedAway);
    printf("Total haircut time was %d minutes\n", (int)(avgHaircut / 60));
    printf("Average stay time was %d minutes\n", (int)(avgStay / 60));

    fprintf(bs.log,"Revenue is %d\n", bs.revenue);
    fprintf(bs.log,"%d Customers have been turned away\n", turnedAway);
    fprintf(bs.log,"Total customers served is %d\n", numCustomers - turnedAway);
    fprintf(bs.log,"Total haircut time was %d minutes\n", (int)(avgHaircut / 60));
    fprintf(bs.log,"Average stay time was %d minutes\n", (int)(avgStay / 60));

   fclose(bs.log);

   return 0;
}

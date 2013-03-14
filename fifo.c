#include "fifo.h"
#include <stdlib.h>
#include <stdio.h>

/* add a new element to a fifo */
void Enqueue( fifo* queue, int customerData)
{
    //Determine if a head or tail exists

    /*For some reason, even if there is no head or tail, C will print out some garbage
    Saying it isn't null.  Instead, we'll look at the garbage value of customerId
    to determine if it actually exists.*/

    int tailExists = 0;

    if(queue->tail != NULL){
        if(queue->tail->customerId >= 0){
            tailExists = 1;
        }
    }

    //Create new elements


    struct fifo_element *element = malloc(sizeof(*element));

    element->customerId = customerData;


    //Set the next element to the current tail
    if(tailExists == 1)
        element->next = queue->tail;
    else
        element->next = NULL;


    //Set the prev element to null
    element->prev = NULL;

    //Set the last element's previous to the new element
    if(tailExists == 1){
        queue->tail->prev = element;
    }

    //Set the tail to the new element
    queue->tail = element;
    //If the head doesn't exist, the new element
    //is also the head
    if(tailExists == 0){
      //  printf("Modifying head\t");
        queue->head = element;
    }

  //  printf("the head is %d\t", queue->head->customerId);
  //  printf("the tail is %d\t", queue->tail->customerId);

}

/* remove an element from a fifo queue */
int Dequeue( fifo* queue )
{
    int tailExists = 0;

    if(queue->tail != NULL){
        if(queue->tail->customerId >= 0){
            tailExists = 1;
        }
    }


    int customerId = -1;
    if(tailExists == 1){

    //Remove the head

        customerId = queue->head->customerId;
       // printf("segfault1\t");
       // fflush(stdout);


        int lastElement = 0;
        //Change the second element to point to NULL
        //This function will only work if there are at least two elements
        if(queue->head->customerId != queue->tail->customerId){
            queue->head->prev->next = NULL;
        }
        else
            lastElement = 1;


       // else{
       //     queue->head = NULL;
      //      queue->tail = NULL;
      //  }
            //    printf("segfault2\t");
     //   fflush(stdout);

        //Set it to the head
        queue->head->customerId = -1;
        queue->head = queue->head->prev;



 /*       if(lastElement){
            queue->head->customerId = -1;
            queue->tail = NULL;
        }*/
        //        printf("segfault3\t");
       // fflush(stdout);
    }

   // printf("Returning %d\t", customerId);
   // fflush(stdout);


    return customerId;

}

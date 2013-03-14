#ifndef FIFO_H
#define FIFO_H

typedef struct fifo_element fifo_element;
typedef struct fifo fifo;

struct fifo_element
{
   int customerId;
   fifo_element *next;
   fifo_element *prev;
};

struct fifo
{
   fifo_element *head;
   fifo_element *tail;
};

/* modify (and complete) the following functions */
void Enqueue( fifo* queue, int customerData );
int Dequeue( fifo* queue );

#endif

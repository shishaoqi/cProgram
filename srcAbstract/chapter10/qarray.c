/*
 * File: qarray.c
 * --------------
 * This file implements the queue.h abstraction using an array.
 */

#include <stdio.h>
#include "genlib.h"
#include "queue.h"

/*
 * Constants:
 * ----------
 * MaxQueueSize   -- Maximum number of elements in the queue
 * QueueArraySize -- Size of the internal array
 */

#define MaxQueueSize   100
#define QueueArraySize (MaxQueueSize + 1)

/*
 * Type: queueCDT
 * --------------
 * This type defines the concrete representation of a queue.
 * This implementation uses a ring buffer to implement the
 * queue.  The next item to be dequeued is found at the array
 * element indexed by head.  The tail index indicates the next
 * free position.  When head and tail are equal, the queue is
 * empty.  The head and tail indices each move from the end of
 * the array back to the beginning, giving rise to the name
 * "ring buffer."  The functions use modular arithmetic to
 * implement this wrap-around behavior.
 */

struct queueCDT {
    queueElementT elements[QueueArraySize];
    int head;
    int tail;
};

/* Exported entries */

queueADT NewQueue(void)
{
    queueADT queue;

    queue = New(queueADT);
    queue->head = queue->tail = 0;
    return (queue);
}

void FreeQueue(queueADT queue)
{
    FreeBlock(queue);
}

void Enqueue(queueADT queue, queueElementT element)
{
    if (QueueIsFull(queue)) Error("Enqueue: queue is full");
    queue->elements[queue->tail] = element;
    queue->tail = (queue->tail + 1) % QueueArraySize;
}

queueElementT Dequeue(queueADT queue)
{
    queueElementT result;

    if (QueueIsEmpty(queue)) Error("Dequeue: queue is empty");
    result = queue->elements[queue->head];
    queue->head = (queue->head + 1) % QueueArraySize;
    return (result);
}

bool QueueIsEmpty(queueADT queue)
{
    return (queue->head == queue->tail);
}

bool QueueIsFull(queueADT queue)
{
    return ((queue->tail + 1) % QueueArraySize == queue->head);
}

/*
 * Implementation note: QueueLength
 * --------------------------------
 * This function determines the number of elements by computing
 * (tail - head) % size.  The size of the queue is added in at
 * the beginning to ensure that the left operand to % is always
 * positive.
 */

int QueueLength(queueADT queue)
{
    return ((QueueArraySize + queue->tail - queue->head)
              % QueueArraySize);
}

queueElementT GetQueueElement(queueADT queue, int index)
{
    if (index < 0 || index >= QueueLength(queue)) {
        Error("Queue element is out of range");
    }
    return (queue->elements[(queue->head + index)
                             % QueueArraySize]);
}

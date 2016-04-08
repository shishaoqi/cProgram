/*
 * File: queue.h
 * -------------
 * This interface defines an abstraction for queues.  In any
 * single application that uses this interface, the values in
 * the queue are constrained to a single type, although it
 * is easy to change that type by changing the definition of
 * queueElementT in this interface.
 */

#ifndef _queue_h
#define _queue_h

#include "genlib.h"

/*
 * Type: queueElementT
 * -------------------
 * The type queueElementT is used in this interface to indicate
 * the type of values that can be stored in the queue.  Here the
 * queue is used to store values of type void *, but that can
 * be changed by editing this definition line.
 */

typedef void *queueElementT;

/*
 * Type: queueADT
 * --------------
 * The queueADT type is defined as a pointer to its concrete
 * counterpart, which is available only to the implementation,
 * not to clients.
 */

typedef struct queueCDT *queueADT;

/*
 * Function: NewQueue
 * Usage: queue = NewQueue();
 * --------------------------
 * This function allocates and returns an empty queue.
 */

queueADT NewQueue(void);

/*
 * Function: FreeQueue
 * Usage: FreeQueue(queue);
 * ------------------------
 * This function frees the storage associated with queue.
 */

void FreeQueue(queueADT queue);

/*
 * Function: Enqueue
 * Usage: Enqueue(queue, element);
 * -------------------------------
 * This function adds element to the end of the queue.
 */

void Enqueue(queueADT queue, queueElementT element);

/*
 * Function: Dequeue
 * Usage: element = Dequeue(queue);
 * --------------------------------
 * This function removes the data value at the head of the queue
 * and returns it to the client.  If the queue is empty, Dequeue
 * calls Error with an appropriate message.
 */

queueElementT Dequeue(queueADT queue);

/*
 * Functions: QueueIsEmpty, QueueIsFull
 * Usage: if (QueueIsEmpty(queue)) . . .
 *        if (QueueIsFull(queue)) . . .
 * -------------------------------------
 * These functions test whether the queue is empty or full.
 */

bool QueueIsEmpty(queueADT queue);
bool QueueIsFull(queueADT queue);

/*
 * Function: QueueLength
 * Usage: n = QueueLength(queue);
 * ------------------------------
 * This function returns the number of elements in the queue.
 */

int QueueLength(queueADT queue);

/*
 * Function: GetQueueElement
 * Usage: element = GetQueueElement(queue, index);
 * -----------------------------------------------
 * This function returns the element at the specified index in the
 * queue, where the head of the queue is defined as index 0. For
 * example, calling GetQueueElement(queue, 0) returns the initial
 * element from the queue without removing it.  If the caller tries
 * to select an element that is out of range, GetQueueElement calls
 * Error.  Note: This function is not a fundamental queue operation
 * and is instead provided mainly to facilitate debugging.
 */

queueElementT GetQueueElement(queueADT queue, int index);

#endif

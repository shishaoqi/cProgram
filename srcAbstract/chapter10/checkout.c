/*
 * File: checkout.c
 * ----------------
 * This program simulates a checkout line, such as one you
 * might encounter in a grocery store.  Customers arrive at
 * the checkout stand and get in line.  Those customers wait
 * in the line until the cashier is free, at which point
 * they are served and occupy the cashier for some period
 * of time.  After the service time is complete, the cashier
 * is free to serve the next customer in the line.
 *
 * In each unit of time, up to the parameter SimulationTime,
 * the following operations are performed:
 *
 * 1. Determine whether a new customer has arrived.
 *    New customers arrive randomly, with a probability
 *    determined by the parameter ArrivalPercentage.
 *
 * 2. If the cashier is busy, note that the cashier has
 *    spent another minute with that customer.  Eventually,
 *    the customer's time request is satisfied, which frees
 *    the cashier.
 *
 * 3. If the cashier is free, serve the next customer in line.
 *    The service time is taken to be a random period between
 *    MinServiceTime and MaxServiceTime.
 *
 * At the end of the simulation, the program displays the
 * parameters and the following computed results:
 *
 * o  The number of customers served
 * o  The average time spent in line
 * o  The average number of people in line
 */

#include <stdio.h>
#include <stdlib.h>
#include "genlib.h"
#include "strlib.h"
#include "random.h"
#include "queue.h"

/* Simulation parameters */

#define SimulationTime     2000
#define ArrivalProbability    0.1
#define MinServiceTime        5
#define MaxServiceTime       15

/*
 * Type: customerT
 * ---------------
 * A customer is represented using a pointer to a record
 * containing the following information:
 *
 * o The customer number (for debugging traces)
 * o The arrival time (to compute the waiting time)
 * o The time required for service
 */

typedef struct {
    int customerNumber;
    int arrivalTime;
    int serviceTime;
} *customerT;

/*
 * Type: simDataT
 * --------------
 * This type stores the data required for the simulation.  The
 * main program declares a variable of this type and then passes
 * it by reference to every other function in the program.
 */

typedef struct {
    queueADT queue;
    customerT activeCustomer;
    int time;
    int numCustomers;
    int numServed;
    long totalWaitTime;
    long totalLineLength;
} simDataT;

/*
 * Debugging option: traceFlag
 * ---------------------------
 * This variable controls whether the simulation produces a
 * debugging trace.
 */

static bool traceFlag = FALSE;

/* Private function declarations */

static void InitializeSimulation(simDataT *sdp);
static void RunSimulation(simDataT *sdp);
static void EnqueueCustomer(simDataT *sdp);
static void ProcessQueue(simDataT *sdp);
static void ServeCustomer(simDataT *sdp);
static void DismissCustomer(simDataT *sdp);
static void ReportResults(simDataT *sdp);

/* Main program */

main()
{
    simDataT simData;

    Randomize();
    InitializeSimulation(&simData);
    RunSimulation(&simData);
    ReportResults(&simData);
}

/*
 * Function: InitializeSimulation
 * Usage: InitializeSimulation(&simData);
 * --------------------------------------
 * This function initializes the simulation data block whose
 * address is passed as the argument.
 */

static void InitializeSimulation(simDataT *sdp)
{
    sdp->queue = NewQueue();
    sdp->activeCustomer = NULL;
    sdp->numServed = 0;
    sdp->totalWaitTime = 0;
    sdp->totalLineLength = 0;
}

/*
 * Function: RunSimulation
 * Usage: RunSimulation(&simData);
 * -------------------------------
 * This function runs the actual simulation.  In each time unit,
 * the program first checks to see whether a new customer arrives.
 * Then, if the cashier is busy (indicated by having a non-NULL
 * pointer in the activeCustomer field), the program decrements
 * the service time counter for that customer.  Finally, if the
 * cashier is free, it serves another customer from the queue
 * and updates the necessary bookkeeping data.
 */

static void RunSimulation(simDataT *sdp)
{
    for (sdp->time = 0; sdp->time < SimulationTime; sdp->time++) {
        if (RandomChance(ArrivalProbability)) {
            EnqueueCustomer(sdp);
        }
        ProcessQueue(sdp);
    }
}

/*
 * Function: EnqueueCustomer
 * Usage: EnqueueCustomer(&simData);
 * ---------------------------------
 * This function simulates the arrival of a new customer.
 */

static void EnqueueCustomer(simDataT *sdp)
{
    customerT c;

    sdp->numCustomers++;
    c = New(customerT);
    c->customerNumber = sdp->numCustomers;
    c->arrivalTime = sdp->time;
    c->serviceTime = RandomInteger(MinServiceTime, MaxServiceTime);
    Enqueue(sdp->queue, c);
    if (traceFlag) {
        printf("%4d: Customer %d arrives and gets in line\n",
               sdp->time, sdp->numCustomers);
    }
}

/*
 * Function: ProcessQueue
 * Usage: ProcessQueue(&simData);
 * ------------------------------
 * This function processes a single time cycle for the queue.
 */

static void ProcessQueue(simDataT *sdp)
{
    if (sdp->activeCustomer == NULL) {
        if (!QueueIsEmpty(sdp->queue)) {
            ServeCustomer(sdp);
        }
    } else {
        if (sdp->activeCustomer->serviceTime == 0) {
            DismissCustomer(sdp);
        } else {
            sdp->activeCustomer->serviceTime--;
        }
    }
    sdp->totalLineLength += QueueLength(sdp->queue);
}

/*
 * Function: ServeCustomer
 * Usage: ServeCustomer(&simData);
 * -------------------------------
 * This function is called when the cashier is free and a
 * customer is waiting.  The effect is to serve the first
 * customer in the line and update the total waiting time.
 */

static void ServeCustomer(simDataT *sdp)
{
    customerT c;

    c = Dequeue(sdp->queue);
    sdp->activeCustomer = c;
    sdp->numServed++;
    sdp->totalWaitTime += (sdp->time - c->arrivalTime);
    if (traceFlag) {
        printf("%4d: Customer %d reaches cashier\n",
               sdp->time, c->customerNumber);
    }
}

/*
 * Function: DismissCustomer
 * Usage: DismissCustomer(&simData);
 * ---------------------------------
 * This function is called when the active customer's service
 * time has dropped to 0. The cashier becomes free and the
 * program no longer needs to hold the customer's storage.
 */

static void DismissCustomer(simDataT *sdp)
{
    if (traceFlag) {
        printf("%4d: Customer %d leaves cashier\n",
               sdp->time, sdp->activeCustomer->customerNumber);
    }
    FreeBlock(sdp->activeCustomer);
    sdp->activeCustomer = NULL;
}

/*
 * Function: ReportResults
 * Usage: ReportResults(&simData);
 * -------------------------------
 * This function reports the results of the simulation.
 */

static void ReportResults(simDataT *sdp)
{
    printf("Simulation results given the following parameters:\n");
    printf("  SimulationTime:     %4d\n", (int) SimulationTime);
    printf("  ArrivalProbability: %7.2f\n",
                                   (double) ArrivalProbability);
    printf("  MinServiceTime:     %4d\n", (int) MinServiceTime);
    printf("  MaxServiceTime:     %4d\n", (int) MaxServiceTime);
    printf("\n");
    printf("Customers served:     %4d\n", sdp->numServed);
    printf("Average waiting time: %7.2f\n",
           (double) sdp->totalWaitTime / sdp->numServed);
    printf("Average line length:  %7.2f\n",
           (double) sdp->totalLineLength / SimulationTime);
}

/* 
 * File:   PriorityQ.c
 * Author: Tempest Parr
 *
 * Created on April 3rd 2016
 */

#include "PriorityQ.h"

/**
 * Returns a pointer to a new priority queue.
 * This function allocates space for the array holding the pointers
 * heads of FIFO queues and also calls constructors for each FIFO queue.
 * 
 * @return a pointer to a new priority queue
 */
PriorityQ_p PriorityQ_construct(void) {
    int error = 0;
    int i = 0;

    //
    // Allocate memory and zero-initialize it in one call
    //
    PriorityQ_p readyQ = (PriorityQ_p) calloc(PRIORITIES_TOTAL, sizeof(FIFOq_p));
    
    //
    // Construct FIFO queues in each array entry
    //
    for (i = 0; i < PRIORITIES_TOTAL; i++) {
        readyQ[i] = FIFOq_construct(&error);
    }
    
    return readyQ;
}

/**
 * Destroys the priority queue. Loops through the array of pointers
 * to FIFO queue heads, destructs them, then frees memory used for
 * the array itself
 *
 * @param readyQ  - priority queue to destroy
 */
void PriorityQ_destruct(PriorityQ_p readyQ) {
    int i;
    int error = 0;
    
    for (i = 0; i < PRIORITIES_TOTAL; i++) {
        if (readyQ[i] != NULL) {
            FIFOq_destruct (readyQ[i], &error);
        }
    }
    
    free(readyQ);
}

/**
 * Checks if priority queue is empty.
 *
 * @param readyQ - priority queue to check
 *
 * @return true if priority queue is empty, false is it's not
 */
bool PriorityQ_is_empty(PriorityQ_p readyQ) {
    int i;
    int error = 0;
    
    for (i = 0; i < PRIORITIES_TOTAL; i++) {
        if ((readyQ[i] != NULL) && (!FIFOq_is_empty(readyQ[i], &error))) {
            return false;
        }
    }
    
    return true;
}

/**
 * Adds a PCB to the priority queue.
 *
 * @param readyQ - priority queue to add the PCB to
 * @param pcbToEnqueue - PCB to add to the priority queue
 */
void PriorityQ_enqueue(PriorityQ_p readyQ, PCB_p pcbToEnqueue) {
    int error = 0;
    Node_p nodeWithPcb = Node_construct(pcbToEnqueue, NULL, &error);
    
    FIFOq_enqueue(readyQ[pcbToEnqueue->priority], nodeWithPcb, &error);
}

/**
 * Remove a PCB from the priority queue.
 *
 * @param readyQ - priority queue to remove the PCB from
 *
 * @return PCB removed from the priority queue
 */
PCB_p PriorityQ_dequeue(PriorityQ_p readyQ) {
    int i;
    int error = 0;

    for (i = 0; i < PRIORITIES_TOTAL; i++) {
        if ((readyQ[i] != NULL) && (!FIFOq_is_empty(readyQ[i], &error))) {
            return FIFOq_dequeue(readyQ[i], &error);
        }
    }

    return NULL;
}

/**
 * Returns a string that represents the priority queue.
 *
 * @param readyQ - priority queue to create a string for
 *
 * @return pointer to a string with the representation of the priority queue
 */
char * PriorityQ_toString(PriorityQ_p readyQ) {
    char * buffer = (char *) calloc(PRIORITY_Q_STRING_BUFFER_SIZE, sizeof(char));
    int totalSize = 0;
    int i = 0;
    int error = 0;
    int stzForFifoQ;
    for (i = 0; i < PRIORITIES_TOTAL; i++) {
        totalSize += snprintf(buffer + totalSize, PRIORITY_Q_STRING_BUFFER_SIZE - totalSize, "Q%d:", i);
        
        // this number is how much space left in the buffer
        stzForFifoQ = PRIORITY_Q_STRING_BUFFER_SIZE - totalSize;
        FIFOq_toString(readyQ[i], buffer + totalSize, &stzForFifoQ, &error);
        
        // stz gets updated by fifoq's tostring, need to recalc totalSize
        totalSize = PRIORITY_Q_STRING_BUFFER_SIZE - stzForFifoQ;
        
        totalSize += snprintf(buffer + totalSize, PRIORITY_Q_STRING_BUFFER_SIZE - totalSize, "\n");
    }
    
    return buffer;
}


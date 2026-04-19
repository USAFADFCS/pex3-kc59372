/** pagequeue.c
 * ===========================================================
 * Name: Kaci McBrayer , 23 Apr 2026
 * Section: CS483 / M3
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    PageQueue* pq = (PageQueue*) malloc(sizeof(PageQueue));
    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;
    pq->maxSize = maxSize;
    return pq;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    //
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    //
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.
    PqNode *current = pq->tail;
    long depth = 0;
    while (current) {
        if (current->pageNum == pageNum) {
            // HIT
            // remove the node
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                pq->head = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                pq->tail = current->prev;
            }
            // insert at tail
            current->prev = pq->tail;
            current->next = NULL;
            if (pq->tail) {
                pq->tail->next = current;
            } else {
                pq->head = current;
            }
            pq->tail = current;
            return depth;
        }
        current = current->prev;
        depth++;
    }
    // MISS
    // allocate new node
    PqNode *newNode = (PqNode *)malloc(sizeof(PqNode));
    newNode->pageNum = pageNum;
    // insert at tail
    newNode->prev = pq->tail;
    newNode->next = NULL;
    if (pq->tail) {
        pq->tail->next = newNode;
    } else {
        pq->head = newNode;
    }
    pq->tail = newNode;
    pq->size++;
    // if size exceeds maxSize, evict head
    if (pq->size > pq->maxSize) {
        PqNode *head = pq->head;
        pq->head = pq->head->next;
        if (pq->head) {
            pq->head->prev = NULL;
        } else {
            pq->tail = NULL;
        }
        free(head);
        pq->size--;
    }
    return -1;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    PqNode* current = pq->head;
    while (current) {
        PqNode* next = current->next;
        free(current);
        current = next;
    }
    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
    PqNode* current = pq->head;
    while (current) {
        printf("%ld ", current->pageNum);
        current = current->next;
    }
    printf("\n");
}

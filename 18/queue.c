#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef struct node {
    int val;
    struct node *next;
} node_t;

queue_t * queue_init(){
    return calloc(sizeof(queue_t),1);
}

void queue_destroy(queue_t * q){
    node_t *itr1 = q->hd;
    node_t *itr2 = q->hd;

    while (itr1) {
        itr2 = itr1->next;
        free(itr1);
        itr1 = itr2;
    }
}

int queue_size(queue_t * q){
    return q->size;
}

void enqueue(queue_t * q, int val) {
    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node) return;

    new_node->val = val;
    new_node->next = q->hd;

    q->hd = new_node;
    q->size++;
}

int dequeue(queue_t * q) {
    node_t *current, *prev = NULL;
    int retval = -1;

    if (q->hd == NULL) return -1;

    current = q->hd;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    retval = current->val;
    free(current);
    
    if (prev)
        prev->next = NULL;
    else
        q->hd = NULL;

    q->size--;
    return retval;
}

void print_list(queue_t * q) {
    node_t *current = q->hd;

    while (current != NULL) {
        printf("%d\n", current->val);
        current = current->next;
    }
}
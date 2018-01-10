#ifndef __DBERGV_QUEUE__
#define __DBERGV_QUEUE__

typedef struct queue_s {
    int size;
    void * hd;
} queue_t;

queue_t * queue_init();

void queue_destroy(queue_t * q);

int queue_size(queue_t * q);

void enqueue(queue_t * q, int val);

int dequeue(queue_t * q);

void print_list(queue_t * q);


#endif /* __DBERGV_QUEUE__ */
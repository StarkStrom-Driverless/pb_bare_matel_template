#include <stdint.h>
#include "ss_can_frame.h"

#ifndef _SS_CAN_RECEIVE_FIFO_H_
#define _SS_CAN_RECEIVE_FIFO_H_

#define FIFO_SIZE 5

struct Fifo {
    struct CanFrame can_frames[FIFO_SIZE];
    int front;
    int rear;
};

void init_fifo(struct Fifo* fifo) {
    fifo->front = -1;
    fifo->rear = -1;
}

uint8_t is_fifo_empty(struct Fifo* fifo) {
    return fifo->front ==  -1;
}

uint8_t is_fifo_full(struct Fifo* fifo) {
    return fifo->rear == FIFO_SIZE - 1;
}

int8_t fifo_add_can_frame(struct Fifo* fifo, struct CanFrame* can_frame) {
    if (is_fifo_full(fifo)) return -1;

    if (fifo->front == -1) fifo->front = 0;

    fifo->rear++;

    fifo->can_frames[fifo->rear].id = can_frame->id;
    fifo->can_frames[fifo->rear].flags = can_frame->flags;
    fifo->can_frames[fifo->rear].dlc = can_frame->dlc;
    for (int i = 0; i < 8; i++) 
        fifo->can_frames[fifo->rear].data[i] = can_frame->data[i];

    return 0;
}

int8_t fifo_remove_can_frame(struct Fifo* fifo, struct CanFrame* can_frame) {
    if (is_fifo_empty(fifo)) return -1;

    can_frame->id = fifo->can_frames[fifo->front].id;
    fifo->can_frames[fifo->front].id = 0;

    can_frame->flags = fifo->can_frames[fifo->front].flags;
    fifo->can_frames[fifo->front].flags = 0;

    can_frame->dlc = fifo->can_frames[fifo->front].dlc;
    fifo->can_frames[fifo->front].dlc = 0;

    for (int i = 0; i < 8; i++) {
        can_frame->data[i] = fifo->can_frames[fifo->front].data[i];
        fifo->can_frames[fifo->front].data[i] = 0;
    }

    if (fifo->front == fifo->rear) {
        fifo->front = -1;
        fifo->rear = -1;
    } else {
        fifo->front++;
    }

    return 0;
}

#endif
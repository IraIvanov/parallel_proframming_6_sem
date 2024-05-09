#pragma once

#include "std_list.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

pthread_mutex_t mutex;

#ifndef STEP_N
#define STEP_N 1000
#endif

#ifndef THREADS
#define THREADS 8
#endif

#ifndef eps
#define eps 0.0001
#endif

volatile double sum = 0.;

struct task{

    double start;
    double end;
    double (* integral_function)(double x);
    struct list_node node;
};

struct thread_info{
    struct list* list;
    int num;
};

double calculate(const double start, const double end, const int steps_num, double (*func)(double x)) {

    double step = (end - start)/steps_num;
    double accum, a, b, left, right = 0;
    for (int i = 0; i < steps_num; i++) {
        left = start + step*i;
        right = start + step*(i + 1);
        a = func(left);
        b = func(right);
        accum += step*(a + b)/2;
    }
    return accum;
}

bool verify_task(const struct task task, int32_t size) {
    
    double step = (task.end - task.start) / STEP_N;
    double start = task.start;
    double end = task.end;
    double error = fabs(calculate(start, end, 1, task.integral_function) - calculate(start, end, 2, task.integral_function));
    return error > (eps/size);
}


void * thread_func(void *arg)
{
    struct list* lst = (struct list*)((struct thread_info*)arg)->list;
    int num = ((struct thread_info*)arg)->num;
    double accumulator = 0.;
    struct list_node* n = NULL;
    int32_t size = 1;
    clock_t start = clock();
    while(size > 0)
    {
        /* Need to safely pop element out of list */
        pthread_mutex_lock(&mutex);
        n = list_pop(lst);
        size = lst->size;
        
        if (n == NULL){
            pthread_mutex_unlock(&mutex);
            break;
        }
        struct task* task = container_of(n, struct task, node);
        bool split = verify_task(*task, size+1);

        if (split)
        {
            double middle = (task->end - task->start)/2 + task->start;
            struct task *subtask= (struct task*)calloc(1, sizeof(struct task));
            subtask->start = task->start;
            subtask->end = middle;
            subtask->integral_function = task->integral_function;
            task->start = middle;
            list_append(lst, &subtask->node);
            list_append(lst, &task->node);
            size += 2;
            pthread_mutex_unlock(&mutex);
            //continue;
        } else {
            pthread_mutex_unlock(&mutex);
            accumulator += calculate(task->start, task->end, STEP_N, task->integral_function);
            free(task);
        }

    }

    clock_t end = clock();
    pthread_mutex_lock(&mutex);
    //list_print(lst);
    sum += accumulator;
    printf("I've finished my work, my time is %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

/*
 * DO SOME SPECIFIC INITIALIZATION HERE
 *
 */

#include "thread_pool.h"
#include "std_list.h"
#include <stdio.h>
#include <stdlib.h>

double i_func(double x){
    return sin(1/x);
}

int main(){

    double start = 0.00001;
    double end = 1;
    struct task *initial_task = (struct task*)calloc(1, sizeof(struct task));
    initial_task->start = start;
    initial_task->end = end;
    initial_task->integral_function = i_func;
    struct list lst;
    list_init(&lst);
    list_append(&lst, &initial_task->node);

    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[THREADS];
    struct thread_info info[THREADS];
    for(int i = 0 ; i < THREADS ;++i ) {
        info[i].list = &lst;
        info[i].num = i;
        pthread_create(&threads[i], NULL, thread_func, (void*)(&info[i]));
    }
    
    for(int i = 0 ; i < THREADS ;++i ){ 
        pthread_join(threads[i], NULL);
    }
    
    printf("%lf\n", sum);
    pthread_mutex_destroy(&mutex);


    return 0;
}

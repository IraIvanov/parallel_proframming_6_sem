#pragma once
//#ifndef STD_LIST_LIB
//#define STD_LIST_LIB

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

#ifndef BUG

#define BUG() \
    printf("ERROR OCCURED, WHILE DOING %s\n", __func__)
#endif


#ifndef offset_of

#define offset_of(type, member) \
                &(((type *)0)->member)
#endif

#ifndef container_of

#define container_of(ptr, type, member) \
               (type *)((char *)ptr - (char *)offset_of(type, member))
#endif




struct list_node{

    struct list_node* prev;
    struct list_node* next;

};

struct list{

    struct list_node* root_node;
    int32_t size;

};

void list_init(struct list* lst) {
    
    lst->root_node = NULL;
    lst->size = 0;
    return;
}

void list_append(struct list* lst, struct list_node* node) {

    if (lst->root_node == NULL) {

        lst->root_node = node;
        node->prev = node;
        node->next = node;
    } else {

        struct list_node* last = lst->root_node->prev;
        last->next = node;
        node->prev = last;
        node->next = lst->root_node;
        lst->root_node->prev = node;
    }
    lst->size++;
    return;
}


struct list_node* list_pop(struct list* lst) {

    struct list_node* node;

    if (lst->size == 0) {
        return NULL;
    }

    if (lst->size < 0) {
        BUG();
        return NULL;
    }

    if (lst->root_node == lst->root_node->prev) {

        node = lst->root_node;
        node->next = NULL;
        node->prev = NULL;
        lst->root_node = NULL;
        lst->size = 0;
    } else {

        node = lst->root_node->prev;
        struct list_node* new_last = node->prev;
        new_last->next = lst->root_node;
        lst->root_node->prev = new_last;
        node->next = NULL;
        node->prev = NULL;
        lst->size--; 
    }
    return node;
}

void list_print(struct list* lst){
    if (lst->root_node == NULL)
        printf("List is empty\n");
    else{
        struct list_node* root = lst->root_node;
        struct list_node* node = root->next;
        printf("root %p->", root);
        while(root != node){
            printf("%p->", node);
            node = node->next;
        }
        printf("root\n");
    }
}
//#endif
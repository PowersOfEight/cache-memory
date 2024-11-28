#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"


linked_list* init_linked_list(void (*destroy_obj)(void*)){
    linked_list* list = (linked_list*) malloc(sizeof(linked_list));
    if (!list) {
        perror("Failed to allocate linked list");
        exit(EXIT_FAILURE);
    }
    list->head = (list_node*) malloc(sizeof(list_node));
    list->tail = (list_node*) malloc(sizeof(list_node));
    if (!list->head || !list->tail) {
        perror("Failed to allocate sentinel nodes");
        free(list);
        exit(EXIT_FAILURE);
    }

    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->head->prev = NULL;
    list->tail->next = NULL;
    list->curr = NULL;
    list->length = 0;
    list->destroy_obj = destroy_obj;
    return list;
}

linked_list* init_simple_linked_list() {
    return init_linked_list(NULL);
}


void reset_front(linked_list* list){
    list->curr = list->length > 0 ? list->head->next : NULL;
}

void reset_back(linked_list* list) {
    list->curr = list->length > 0 ? list->tail->prev : NULL;
}

int next(linked_list* list) {
    if (list->curr && list->curr->next != list->tail) {
        list->curr = list->curr->next;
        return 1;
    } else {
        return 0;
    }
}

int previous(linked_list* list) {
    if (list->curr && list->curr->prev != list->head) {
        list->curr = list->curr->prev;
        return 1;
    } else {
        return 0;
    }
}

unsigned long list_append(linked_list* list, void* obj) {
    if(!list) return 0;
    list_node* new_node = malloc(sizeof(list_node));
    if (!new_node) {
        perror("Failed to allocate new node for append operation");
        exit(EXIT_FAILURE);
    }

    new_node->object = obj;

    new_node->prev=list->tail->prev;
    new_node->next=list->tail;

    list->tail->prev->next = new_node;
    list->tail->prev = new_node;

    if (list->length == 0) {
        list->curr = new_node;
    }

    return ++list->length;
}

void* get_curr(linked_list* list) {
    if(!list || !list->curr) return NULL;
    return list->curr->object;
}

void destroy_linked_list(linked_list* list){
    if (!list) return;

    list_node* curr = list->head->next;
    
    while (curr != list->tail) {
        list_node* tmp = curr;
        curr = tmp->next;
        tmp->next->prev = tmp->prev;
        tmp->prev->next = tmp->next;
        if (list->destroy_obj) {
            list->destroy_obj(tmp->object);
        } else {
            free(tmp->object);
        }
        free(tmp);
    }
    free(list->tail);
    free(list->head);
    free(list);
}

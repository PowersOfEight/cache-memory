#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"


linked_list* init_linked_list(){
    linked_list* list = (linked_list*) malloc(sizeof(linked_list));
    list->head = (list_node*) malloc(sizeof(list_node));
    list->tail = (list_node*) malloc(sizeof(list_node));
    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->length = 0;
    return list;
}

unsigned long append(linked_list* list, void* obj) {
    if(!list) return 0;
    list_node* new_node = malloc(sizeof(list_node));
    new_node->object = obj;

    new_node->prev=list->tail->prev;
    new_node->next=list->tail;

    list->tail->prev->next = new_node;
    list->tail->prev = new_node;
    return ++list->length;
}

void destroy_linked_list(linked_list* list){
    list_node* curr = list->head->next;
    while (curr != list->tail) {
        // remove and free listnodes
        list_node* tmp = curr;
        curr = tmp->next;
        tmp->next->prev = tmp->prev;
        tmp->prev->next = tmp->next;
        free(tmp->object);
        free(tmp);
    }
    free(list->tail);
    free(list->head);
    free(list);
}

int main(int argc, char** argv) {
    linked_list* list = init_linked_list();

    // Copy each argument into the linked list
    for (int i = 0; i < argc; i++) { // Skip argv[0] (program name)
        char* arg_copy = (char*) malloc(strlen(argv[i]) + 1); // Allocate memory for the string
        if (!arg_copy) {
            perror("malloc failed");
            destroy_linked_list(list);
            return 1; // Exit if malloc fails
        }
        strcpy(arg_copy, argv[i]); // Copy the argument into allocated memory
        append(list, arg_copy);   // Append the copied string to the list
    }

    // Print list contents
    list_node* curr = list->head->next;
    printf("Length of list: %lu\n", list->length);
    printf("Arguments in the list:\n");
    while (curr != list->tail) {
        printf("%s\n", (char*)curr->object);
        curr = curr->next;
    }

    // Destroy the list (frees all memory)
    destroy_linked_list(list);
    return 0;
}
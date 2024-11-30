#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"


void print_forward(linked_list* list) {
    reset_front(list);
    char* str;
    printf("Printing strings...\n");
    while ((str = (char*) get(list)) != NULL ) {
        printf("%s\n", str);
        if (!next(list)) break;
    }
}
void print_backward(linked_list* list) {
    reset_back(list);
    char* str;
    printf("Printing strings back to front...\n");
    while ((str = (char*) get(list)) != NULL) {
        printf("%s\n", str);
        if (!previous(list)) break;
    }
}

int main(int argc, char** argv) {
    linked_list* list = init_simple_linked_list();

    for (int i = 0; i < argc; i++) {
        char* arg = (char*) malloc(strlen(argv[i] + 1));
        strcpy(arg, argv[i]);
        append_list(list, arg);
    }
    print_forward(list);
    print_backward(list);
    destroy_linked_list(list);
    printf("Done...\n");
}
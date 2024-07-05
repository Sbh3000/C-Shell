#include "headers.h"

// Initialize a new node
struct bg_list* init(int index, int pid, char** command) {
    struct bg_list* node = (struct bg_list*)malloc(sizeof(struct bg_list));
    node->status = "Running";
    node->index = index;
    node->pid = pid;
    node->command = command;
    node->next = NULL;
    return node;
}

// Add a new node to the list
struct bg_list* add(struct bg_list* head, int index, int pid, char** command) {
    struct bg_list* new_node = init(index, pid, command);
    
    if (head == NULL) {
        return new_node;
    }
    struct bg_list* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
    return head;
}

// Delete a node from the list
struct bg_list* delete(struct bg_list* head, int index) {
    if (head == NULL) {
        return NULL;
    }
    if (head->index == index) {
        struct bg_list* temp = head->next;
        free(head);
        return temp;
    }
    struct bg_list* current = head;
    while (current->next != NULL && current->next->index != index) {
        current = current->next;
    }
    if (current->next != NULL) {
        struct bg_list* temp = current->next;
        current->next = current->next->next;
        free(temp);
    }
    return head;
}

#include <lcom/lcf.h>

#include "list.h"

List create_list(){
    List list = {.size = 0, .head = NULL, .tail = NULL};
    return list;
}

void* get_front(List* list){
    return (list->size) ? list->head->value : NULL;
}

void* get_back(List* list){
    return (list->size) ? list->tail->value : NULL;
}

void push_back(List* list, void* value){
    // create the new element
    ListElement* new_element = (ListElement*) malloc(sizeof *new_element);
    new_element->value = value;
    new_element->next = NULL;
    new_element->prev = list->tail;

    // update the list
    if (!list->size) list->tail->next = new_element;
    if (!list->size) list->head = new_element;
    list->tail = new_element;

    ++list->size;
}

void push_front(List* list, void* value){
    // create the new element
    ListElement* new_element = (ListElement*) malloc(sizeof *new_element);
    new_element->value = value;
    new_element->next = list->head;
    new_element->prev = NULL;

    // update the list
    if (!list->size) list->head->prev = new_element;
    list->head = new_element;
    if (!list->size) list->tail = new_element;

    ++list->size;
}

void pop_back(List* list){
    if (!list->size) return;

    // update the list
    --list->size;

    ListElement* old_tail = list->tail;
    list->tail = old_tail->prev;

    if (list->size)
        list->tail->next = NULL;
    else
        list->head = NULL;

    free(old_tail);
}

void pop_front(List* list){
    if (!list->size) return;

    // update the list
    --list->size;

    ListElement* old_head = list->head;
    list->head = old_head->next;

    if (list->size)
        list->head->prev = NULL;
    else
        list->tail = NULL;

    free(old_head);
}

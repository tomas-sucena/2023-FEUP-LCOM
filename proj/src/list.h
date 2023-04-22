#ifndef _LCOM_LIST_H_
#define _LCOM_LIST_H_

typedef struct ListElement {
    void* value;
    struct ListElement* prev;
    struct ListElement* next;
} ListElement;

typedef struct {
    ListElement* head;
    ListElement* tail;
    int size;
} List;

List create_list();

/* GETTERS */
void* get_front(List* list);
void* get_back(List* list);

void push_back(List* list, void* value);
void push_front(List* list, void* value);
void pop_back(List* list);
void pop_front(List* list);

#endif // _LCOM_LIST_H_

#include <stddef.h> 
#include <stdio.h>
#include <stdlib.h>

struct list_node {
    struct list_node *next;
};




struct list_node *list_get_tail(struct list_node **head) {
    if (!head) {
        return NULL;
    }
    struct list_node *current_node = *head;
    struct list_node *last_node = NULL;
    while (current_node) {
        last_node = current_node;
        current_node = current_node->next;
    }
    return last_node;
}

struct list_node *list_append(struct list_node **head, struct list_node *item) {
    struct list_node *tail = list_get_tail(head);
    if (!tail) {
        *head = item;
    } else {
        tail->next = item;
    }
    item->next = NULL;
    return item;
}

struct list_node *list_pop(struct list_node **head) {
    struct list_node *current_head = *head;
    if (!current_head) {
        return NULL;
    }
    *head = (*head)->next;
    current_head->next = NULL;
    return current_head;
}

struct int_item {
    struct list_node node;
    int value;
};

struct int_item *int_item_new(int value) {
    struct int_item *item = malloc(sizeof(struct int_item));
    if (!item) {
        return NULL;
    }
    item->value = value;
    return item;
}

int list_remove_item(struct list_node **head, int value) {
    struct list_node *current = *head;
    struct list_node *prev = NULL;

    while (current != NULL) {
        struct int_item *int_item = (struct int_item *)current;
        if (int_item->value == value) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}

void list_reverse(struct list_node **head) {
    struct list_node *prev = NULL;
    struct list_node *current = *head;
    struct list_node *next = NULL;

    while (current != NULL) {
        next = current->next;  
        current->next = prev;  
        prev = current;        
        current = next;
    }

    *head = prev; 
}


void print_list(struct int_item *head) {
    struct int_item *current_item = head;
    while (current_item != NULL) {
        printf("%d\n", current_item->value);
        current_item = (struct int_item *)current_item->node.next;
    }
}


#define TO_LIST_NODE(item) ((struct list_node *)(item))
#define TO_INT_ITEM(node) ((struct int_item *)(node))
#define LIST_PTR_PTR(head)((struct list_node **)(head))
#define NEW_INT_ITEM(value) int_item_new(value)
#define APPEND_INT_ITEM(head, value) list_append(LIST_PTR_PTR(head), TO_LIST_NODE(NEW_INT_ITEM(value)))
#define REMOVE_INT_ITEM(head, value) list_remove_item(LIST_PTR_PTR(head), value)

int main() {
    struct int_item *my_linked_list = NULL;

    APPEND_INT_ITEM(&my_linked_list, 10);
    APPEND_INT_ITEM(&my_linked_list, 20);
    APPEND_INT_ITEM(&my_linked_list, 30);
    APPEND_INT_ITEM(&my_linked_list, 40);

    printf("Lista originale:\n");
    print_list(my_linked_list);

    int value_to_remove = 10;
    if (REMOVE_INT_ITEM(&my_linked_list, value_to_remove)) {
        printf("Elemento rimosso: %d\n", value_to_remove);
    } else {
        printf("Elemento non trovato: %d\n", value_to_remove);
    }
    printf("Lista con rimozione:\n");
    print_list(my_linked_list);

    list_reverse(LIST_PTR_PTR(&my_linked_list));

    printf("Lista invertita:\n");
    print_list(my_linked_list);

    return 0;
}


#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


struct list_node
{
    struct list_node *prev;
    struct list_node *next;
    int data;
};

struct list_node *list_get_tail(struct list_node **head) 
{
    if (!head || !*head) return NULL; 
    
    struct list_node *current = *head;
    while (current->next) 
    {
        current = current->next;
    }
    return current; 
}

struct list_node *list_append (struct list_node **head, struct list_node *item)
{
    struct list_node *tail = list_get_tail (head);

    if (!tail)
    {
        * head = item;
    }
    else
    {
        tail->next = item;
    }
    item->prev = tail;
    item->next = NULL;
    return item;
}

struct list_node *create_node(int data)
{
    struct list_node *new_node = malloc(sizeof(struct list_node));
    
    if(new_node)
    {
        new_node->prev = NULL;
        new_node->next = NULL;
        new_node->data = data;

    }

    return new_node;
}

void list_remove(struct list_node **head, int data) 
{
    if (!head || !*head) 
    {
        return; 
    }

    struct list_node *current_node = *head;

    while (current_node) 
    {
        if (current_node->data == data) 
        {
            if (current_node->prev) 
            {
                current_node->prev->next = current_node->next;
            } else 
            {
                *head = current_node->next;
            }

            if (current_node->next) 
            {
                current_node->next->prev = current_node->prev;
            }

            free(current_node);
            return; 
        }

        current_node = current_node->next; 
    }
}

void list_insert_after(struct list_node **head, int data_to_insert, int data_target) {
    if (!head || !*head) 
    {
        return; 
    }

    struct list_node *current_node = *head;

    while (current_node) 
    {
        if (current_node->data == data_target) 
        {
            struct list_node *new_node = create_node(data_to_insert);

            new_node->next = current_node->next; 
            new_node->prev = current_node;       

            if (current_node->next) 
            {
                current_node->next->prev = new_node; 
            }

            current_node->next = new_node; 

            return; 
        }

        current_node = current_node->next; 
    }
}

void list_insert_before(struct list_node **head, int data_to_insert, int data_target) 
{
    if (!head || !*head) 
    {
        return; 
    }

    struct list_node *current_node = *head;

    while (current_node) 
    {
        if (current_node->data == data_target) {
            struct list_node *new_node = create_node(data_to_insert);

            new_node->prev = current_node->prev;
            new_node->next = current_node;

            if (current_node->prev) 
            {
                current_node->prev->next = new_node; 
            }

            current_node->prev = new_node; 

            return; 
        }

        current_node = current_node->next; 
    }
}

void list_shuffle(struct list_node **head) 
{
    if (!head || !*head || !(*head)->next) 
    {
        return; 
    }

    
    int count = 0;
    struct list_node *current = *head;
    while (current) 
    {
        count++;
        current = current->next;
    }

    
    struct list_node **array = malloc(count * sizeof(struct list_node *));
    if (!array) 
    {
        return; 
    }

   
    current = *head;
    for (int i = 0; i < count; i++) 
    {
        array[i] = current;
        current = current->next;
    }

    
    srand(time(NULL)); 
    for (int i = count - 1; i > 0; i--) 
    {
        int j = rand() % (i + 1);

        
        struct list_node *temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    
    for (int i = 0; i < count - 1; i++) 
    {
        array[i]->next = array[i + 1];
        array[i + 1]->prev = array[i];
    }
    array[0]->prev = NULL;
    array[count - 1]->next = NULL;

    
    *head = array[0];

    free(array); 
}



void print_list(struct list_node *head)
{
    struct list_node *current_node = head;
    while((current_node))
    {
        printf("%d \n", current_node->data);

        
        current_node = current_node -> next;
    }
    
}


int main()
{
    struct list_node *my_doubly_linked_list = NULL;
    list_append(&my_doubly_linked_list, create_node(1));
    list_append(&my_doubly_linked_list, create_node(2));
    list_append(&my_doubly_linked_list, create_node(3));
    list_append(&my_doubly_linked_list, create_node(4));

    printf("Lista inziale : \n");
    print_list(my_doubly_linked_list);

     int data_to_remove = 3;
     list_remove(&my_doubly_linked_list, data_to_remove);

     printf("Lista con rimozione di %d \n", data_to_remove);
     print_list(my_doubly_linked_list);

     int data_to_insert_after = 10;
     list_insert_after(&my_doubly_linked_list, data_to_insert_after, 2);

     printf("Lista con inserimento di %d \n", data_to_insert_after);
     print_list(my_doubly_linked_list);

     int data_to_insert_before = 10;
     list_insert_before(&my_doubly_linked_list, data_to_insert_before, 2);

     printf("Lista con inserimento di %d \n", data_to_insert_before);
     print_list(my_doubly_linked_list);

    printf("Lista prima dello shuffle: \n");
    print_list(my_doubly_linked_list);

    list_shuffle(&my_doubly_linked_list);

    printf("Lista dopo lo shuffle: \n");
    print_list(my_doubly_linked_list);


    return 0;
}
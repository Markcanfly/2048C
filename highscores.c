#include <string.h>
#include "debugmalloc.h"
#include "highscores.h"

/**
* \brief Find a node
* Finds the node with the specified parameters, and return its adress
* \param first Pointer pointer pointing to the first node of our list
* \param name The name we're looking to find
* \param field_size The field size we're looking to find
*/
HS_Node *find_node(HS_Node *first, const char *name, const int field_size) {
    for (HS_Node *current = first; current != NULL; current = current -> next) {
        if (strcmp(current -> name, name) == 0 && current -> field_size == field_size)
            return current;
    }
    // Not found
    return NULL;
}

/**
* \brief Find a node, and get its predecessor
* Finds the node with the specified parameters, and returns the adress of the previous
* \param first Pointer pointer pointing to the first node of our list
* \param name The name we're looking to find
* \param field_size The field size we're looking to find
*/
HS_Node *find_previous_node(HS_Node *first, const char *name, const int field_size) {
    for (HS_Node *current = first; current -> next != NULL; current = current -> next) {
        if (strcmp(current -> next -> name, name) == 0 && current -> next -> field_size == field_size)
            return current;
    }
    // Not found
    return NULL;
}

/**
* \brief Delete a highscore node
* \param first Pointer pointer pointing to the first node of our list
* \param name The name we're looking to delete the node for
* \param field_size The field size we're looking to delete the node for
*/
void del_highscore(HS_Node **first, const char *name, const int field_size) {
    if (strcmp((*first) -> name, name) == 0 && (*first) -> field_size == field_size) {
        // Check if the first object is the one needed to be deleted
        HS_Node *new_first = (*first) -> next;
        free(*first);
        *first = new_first;
    } else {
        // Otherwise delete the required object
        HS_Node *node = find_previous_node(*first, name, field_size);
        if (node) { // is not null
            HS_Node *new_next = node -> next -> next;
            free(node -> next);
            node -> next = new_next;
        }
    }
}

/**
* \brief Add a highscore
* \param first Pointer pointer pointing to the first node of our list
* \param name The name we're looking to delete the node for
* \param field_size The field size we're looking to delete the node for
* \param score The score looking to be added to the list
* Adds a highscore node to the appropriate place in the linked list
* Unconditional, unlike add_checked_highscore
*/
void add_highscore(HS_Node **first, char *name, int field_size, int score) {
    // Check if the new score is the highest
    if (*first == NULL) {
        // Just create new node
        *first = (HS_Node *) malloc(sizeof(HS_Node));
        strcpy((*first) -> name, name);
        (*first) -> field_size = field_size;
        (*first) -> score = score;
        (*first) -> next = NULL;
    } else if ((*first) -> score < score) {
        HS_Node *new_next = *first;
        *first = (HS_Node *) malloc(sizeof(HS_Node));
        strcpy((*first) -> name, name);
        (*first) -> field_size = field_size;
        (*first) -> score = score;
        (*first) -> next = new_next;
    } else {
        // Find the smallest score that's still larger than ours
        HS_Node *previous = *first;
        while (previous -> next != NULL && previous -> next -> score > score) {
            previous = previous -> next;
        }
        // Now the previous pointer points to the object before our new score
        // Insert new node here
        HS_Node *new_next = previous -> next;
        previous -> next = (HS_Node *) malloc(sizeof(HS_Node));
        strcpy(previous -> next -> name, name);
        previous -> next -> field_size = field_size;
        previous -> next -> score = score;
        previous -> next -> next = new_next;
    }
}

/**
* \brief Add/Update highscore
* \param first Pointer pointer pointing to the first node of our list
* \param name The name we're looking to delete the node for
* \param field_size The field size we're looking to delete the node for
* \param score The score looking to be added to the list
* Check if user already has a high score in this field size,
* if so, delete that, then add a new node one to the appropriate place in the linked list.
*/
void add_checked_highscore(HS_Node **first, char *name, int field_size, int score) {
    del_highscore(first, name, field_size);
    add_highscore(first, name, field_size, score);
}

void debug_print_HS(HS_Node *node) {
    while (node != NULL) {
        printf("%d | %d | %s\n", node -> score, node -> field_size, node -> name);
        node = node -> next;
    }
}

/**
* \brief Free all highscores linked to a node
*/
void free_highscores(HS_Node **node) {
    while (*node != NULL) {
        HS_Node *next = (*node) -> next;
        free(*node);
        *node = next;
    }
}

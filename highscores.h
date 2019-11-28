#ifndef HIGHSCORES_H_INCLUDED
#define HIGHSCORES_H_INCLUDED

/**
* \brief Linked list to store the highscores
* It is intended to be always sorted, and only traversed through for search of a name, or display of top 10 scores.
*/
typedef struct HS_Node {
    char name[51];
    int field_size;
    int score;
    struct HS_Node *next;
} HS_Node;

/**
* \brief Find a node
* Finds the node with the specified parameters, and return its adress
* \param first Pointer pointer pointing to the first node of our list
* \param name The name we're looking to find
* \param field_size The field size we're looking to find
*/
HS_Node *find_node(HS_Node *first, const char *name, const int field_size);

/**
* \brief Find a node, and get its predecessor
* Finds the node with the specified parameters, and returns the adress of the previous
* \param first Pointer pointer pointing to the first node of our list
* \param name The name we're looking to find
* \param field_size The field size we're looking to find
*/
HS_Node *find_previous_node(HS_Node *first, const char *name, const int field_size);

/**
* \brief Delete a highscore node
* \param first Pointer pointer pointing to the first node of our list
* \param name The name we're looking to delete the node for
* \param field_size The field size we're looking to delete the node for
*/
void del_highscore(HS_Node **first, const char *name, const int field_size);

/**
* \brief Deletes a node, given the node before it
* \param previous The node directly before the one to be deleted.
*/
void del_node_after(HS_Node *previous);

/**
* \brief Add a highscore
* \param first Pointer pointer pointing to the first node of our list
* \param name The name we're looking to delete the node for
* \param field_size The field size we're looking to delete the node for
* \param score The score looking to be added to the list
* Adds a highscore node to the appropriate place in the linked list
* Unconditional, unlike add_checked_highscore
*/
void add_highscore(HS_Node **first, char *name, int field_size, int score);

/**
* \brief Add/Update highscore
* \param first Pointer pointer pointing to the first node of our list
* \param name The name we're looking to delete the node for
* \param field_size The field size we're looking to delete the node for
* \param score The score looking to be added to the list
* Check if user already has a high score in this field size,
* if so, delete that, then add a new node one to the appropriate place in the linked list.
*/
void add_checked_highscore(HS_Node **first, char *name, int field_size, int score);

void debug_print_HS(HS_Node *node);

/**
* \brief Free all highscores linked to a node
*/
void free_highscores(HS_Node **node);

#endif // HIGHSCORES_H_INCLUDED

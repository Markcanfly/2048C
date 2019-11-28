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

HS_Node *find_node(HS_Node *first, const char *name, const int field_size);

HS_Node *find_previous_node(HS_Node *first, const char *name, const int field_size);

void del_highscore(HS_Node **first, const char *name, const int field_size);

void del_node_after(HS_Node *previous);

void add_highscore(HS_Node **first, char *name, int field_size, int score);

void add_checked_highscore(HS_Node **first, char *name, int field_size, int score);

void debug_print_HS(HS_Node *node);

void free_highscores(HS_Node **node);

#endif // HIGHSCORES_H_INCLUDED

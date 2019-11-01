#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

// HYPERPARAMETERS

struct menu_item {
    int id;
    char* title;
    void (action)();
};


#endif // MENU_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include "push_tiles.h"
#include "game.h"

int my_rand(int from, int to) {
    return rand() % (to - from) + from;
}

void push_up(tabla *to_push) {
    int size_x = to_push -> size_x;
    int size_y = to_push -> size_y;
    int **fields = to_push -> dynarr;
    bool valid_move = false;

    for (int oszlop = 0; oszlop < size_x; oszlop++) {
        int i = 1; // Don't check the first
        while (i >= 0 && i <= size_y-1) {
            int *num = &fields[i][oszlop];

            if (i != 0) {
                int *near_num = &fields[i-1][oszlop];
                if (*num == 0) {
                    // num is empty => skip
                    i++;
                } else if (*near_num == 0) {
                    *near_num = *num;
                    *num = 0;
                    valid_move = true;
                    i--;
                    // Decrement, check last num
                } else if (*near_num == *num) {
                    *near_num += *num;

                    // Win check
                    if (*near_num == 2048)
                        printf("Game won.");

                    *num = 0;
                    valid_move = true;
                    i++;
                } else {
                    // Different numbers => skip
                    i++;
                }
            } else {
                // Skip if num is at edge of field
                i++;
            }
        }
    }
    if (valid_move)
        add_random(to_push);
    if (lost(to_push))
        printf("Game lost.");
}
void push_down(tabla *to_push) {
    int size_x = to_push -> size_x;
    int size_y = to_push -> size_y;
    int **fields = to_push -> dynarr;
    bool valid_move = false;

    for (int oszlop = 0; oszlop < size_x; oszlop++) {
        int i = size_y - 2; // Don't check the first
        while (i >= 0 && i <= size_y-1) {
            int *num = &fields[i][oszlop];
            if (i < size_y - 1) {
                int *near_num = &fields[i+1][oszlop];
                if (*num == 0) {
                    // num is empty => skip
                    i--;
                } else if (*near_num == 0) {
                    *near_num = *num;
                    *num = 0;
                    valid_move = true;
                    i++;
                    // Decrement, check last num
                } else if (*near_num == *num) {
                    *near_num += *num;

                    // Win check
                    if (*near_num == 2048)
                        printf("Game won.");

                    *num = 0;
                    valid_move = true;
                    i--;
                } else {
                    // Different numbers => skip
                    i--;
                }
            } else {
                // Skip if i is at edge of field
                i--;
            }
        }
    }
    if (valid_move)
        add_random(to_push);
    if (lost(to_push))
        printf("Game lost.");
}
void push_left(tabla *to_push) {
    int size_x = to_push -> size_x;
    int size_y = to_push -> size_y;
    int **fields = to_push -> dynarr;
    bool valid_move = false;

    for (int sor = 0; sor < size_y; sor++) {
        int i = 1; // Don't check the first
        while (i >= 0 && i <= size_x-1) {
            int *num = &fields[sor][i];

            if (i != 0) {
                int *near_num = &fields[sor][i-1];
                if (*num == 0) {
                    // num is empty => skip
                    i++;
                } else if (*near_num == 0) {
                    *near_num = *num;
                    *num = 0;
                    valid_move = true;
                    i--;
                    // Decrement, check last num
                } else if (*near_num == *num) {
                    *near_num += *num;

                    // Win check
                    if (*near_num == 2048)
                        printf("Game won.");

                    *num = 0;
                    valid_move = true;
                    i++;
                } else {
                    // Different numbers => skip
                    i++;
                }
            } else {
                // Skip if num is at edge of field
                i++;
            }
        }
    }
    if (valid_move)
        add_random(to_push);
    if (lost(to_push))
        printf("Game lost.");
}
void push_right(tabla *to_push) {
    int size_x = to_push -> size_x;
    int size_y = to_push -> size_y;
    int **fields = to_push -> dynarr;
    bool valid_move = false;

    for (int sor = 0; sor < size_y; sor++) {
        int i = size_x - 2; // Don't check the last one
        while (i >= 0 && i <= size_x-1) {
            int *num = &fields[sor][i];

            if (i < size_x - 1) {
                int *near_num = &fields[sor][i+1];
                if (*num == 0) {
                    // num is empty => skip
                    i--;
                } else if (*near_num == 0) {
                    // Is 0, just push
                    *near_num = *num;
                    *num = 0;
                    valid_move = true;
                    i++;
                } else if (*near_num == *num) {
                    // The same number => add
                    *near_num += *num;

                    // Win check
                    if (*near_num == 2048)
                        printf("Game won.");

                    *num = 0;
                    valid_move = true;
                    i--;
                } else {
                    // Different numbers => skip
                    i--;
                }
            } else {
                // Skip if num is at edge of field
                i--;
            }
        }
    }
    if (valid_move)
        add_random(to_push);
    if (lost(to_push))
        printf("Game lost.");
}

void add_random(tabla *to_add) {
    int size_x = to_add -> size_x;
    int size_y = to_add -> size_y;
    int **fields = to_add -> dynarr;

    int* uresek[size_x*size_y];
    int elemcount = 0;

    // Find all empty blocks
    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            if (fields[y][x] == 0) {
                uresek[elemcount++] = &fields[y][x];
            }
        }
    }
    if (elemcount > 0) {
        // !Don't forget! elemcount = max index + 1
        int *found = uresek[my_rand(0, elemcount)];
        *found = (my_rand(0, 10) >= 8) ? 4 : 2;
        // Spawn a 2/4 with a 90/10 % chance.
    }
}


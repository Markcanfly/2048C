#include "style.h"
#include <math.h>

/* Global array of vals for styling
    the index in the array is the log2(n) of the tile num,
    except when it's >2048, in which case it's 0 (black).
*/
rect_style tiles[12] = {
    {{0, 0, 0, 255}, 0xFFFFFFFF}, // super
    {{119, 110, 101, 255}, 0xEEE4DAFF}, // 2
    {{119, 110, 101, 255}, 0xEDE0C8FF}, // 4
    {{249, 246, 242, 255}, 0xF2B179FF}, // 8
    {{249, 246, 242, 255}, 0xF59563FF}, // 16
    {{249, 246, 242, 255}, 0xF67C5FFF}, // 32
    {{249, 246, 242, 255}, 0xF65E3BFF}, // 64
    {{249, 246, 242, 255}, 0xECDF72FF}, // 128
    {{249, 246, 242, 255}, 0xEDCC61FF}, // 256
    {{249, 246, 242, 255}, 0xEDC850FF}, // 512
    {{249, 246, 242, 255}, 0xEDC53FFF}, // 1024
    {{249, 246, 242, 255}, 0xEDC22EFF} // 2048
};

rect_style getTile(int tile_num) {
    if (tile_num > 2048)
        return tiles[0];

    return tiles[(int) log2(tile_num)];
}

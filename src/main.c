#include <stdlib.h>
#include <stdbool.h>

#include "display/display.h"
#include "display/primitives.h"
#include "utils/utils.h"

int main() {
    display_init();

    while (true) {
        int x = randint(0, 310);
        int y = randint(0, 230);
        
        op_t op = {
            .type = OP_RECT,
            .rect = { x, y, 5, 10 },
            .color = 0xFFFF,
        };

        erase_last_frame();
        draw(op);
        swap_buffer_blocking();
    }
}

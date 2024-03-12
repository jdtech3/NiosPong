#include "display/display.h"

// -- Global vars

stack_t* _display__op_buf;
int _display__pixel_buf_start;
short _display__buffer1[PIXEL_BUF_HEIGHT][PIXEL_BUF_WIDTH];
short _display__buffer2[PIXEL_BUF_HEIGHT][PIXEL_BUF_WIDTH];

// -- Functions

void display_init() {
    volatile int* pixel_ctrl_ptr = (int*) PIXEL_CONTROLLER_ADDR;
    
    // Init frame buffers
    *(pixel_ctrl_ptr + 1) = (int) &_display__buffer1;
    swap_buffer_blocking();
    clear_screen(*pixel_ctrl_ptr);

    *(pixel_ctrl_ptr + 1) = (int) &_display__buffer2;
    clear_screen(*(pixel_ctrl_ptr + 1));

    // Set ptr to back buffer
    _display__pixel_buf_start = *(pixel_ctrl_ptr + 1);

    // Init op buffer
    _display__op_buf = stack_init();
}

void draw(op_t op) {
    switch (op.type) {
        case OP_PIXEL:
            draw_pixel(_display__pixel_buf_start, op.pixel, op.color);
            break;

        case OP_LINE:
            draw_line(_display__pixel_buf_start, op.line, op.color);
            break;

        case OP_RECT:
            draw_rect(_display__pixel_buf_start, op.rect, op.color);
            break;
    }

    op_t* mem_op = (op_t*) malloc(sizeof(op_t));
    *mem_op = op;
    stack_push(_display__op_buf, mem_op);
}

void erase_last_frame() {
    op_t* op = stack_pop(_display__op_buf);
    while (op != NULL) {
        op->color = 0x0000;     // blank
        draw(*op);

        op = stack_pop(_display__op_buf);
    }
}

void op_buf_deinit() {
    stack_deinit(_display__op_buf);
}

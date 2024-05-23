#ifndef __SC_ST7735_H
#define __SC_ST7735_H

#include "sc_common.h"
#include "sc_painter.h"

typedef int (*st7735_adaptor_write_data_16_fn_t)(void *adaptor, int data);
typedef int (*st7735_adaptor_write_data_fn_t)(void *adaptor, int data);
typedef int (*st7735_adaptor_write_cmd_fn_t)(void *adaptor, int data);

struct st7735_adaptor_i {
	st7735_adaptor_write_data_16_fn_t write_data_16;
	st7735_adaptor_write_data_fn_t write_data;
	st7735_adaptor_write_cmd_fn_t write_cmd;
};

struct st7735_screen {
	struct drawing_i *drawing_board;
	struct st7735_adaptor_i **adaptor;
	struct point size;
};

int st7735_init(struct st7735_screen *self, struct st7735_adaptor_i **adaptor);

int st7735_set_address(struct st7735_screen *self, struct point p1, struct point p2);

#endif

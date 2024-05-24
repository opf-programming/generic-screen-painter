#ifndef __SC_ST7789_CH32V_FSMC_H
#define __SC_ST7789_CH32V_FSMC_H

#include "sc_adaptor.h"

/// A17 is controlling the DATA/CMD signal (the `DCX` Pin). 0 -> cmd, 1 -> data
#define ST7789_LCD_CMD ((uintptr_t)0x60000000)
#define ST7789_LCD_DATA ((uintptr_t)0x60020000)

struct st7789_adaptor_ch32v_fsmc {
	struct sc_adaptor_i *adaptor;
};

int st7789_adaptor_ch32v_fsmc_init(struct st7789_adaptor_ch32v_fsmc *self);

#endif

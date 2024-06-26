#include "sc_byte_adaptor_ch32v_hwi2c.h"
#include "sc_ch32v_i2c.h"
#include "sc_common.h"

static int start_transmit(struct sc_byte_adaptor_ch32v_hwi2c *self);
static int stop_transmit(struct sc_byte_adaptor_ch32v_hwi2c *self);
static int write_byte(struct sc_byte_adaptor_ch32v_hwi2c *self, int data);

static struct sc_byte_adaptor_i adaptor_interface = {
	.start_transmit = (sc_adaptor_start_transmit_fn_t)start_transmit,
	.stop_transmit = (sc_adaptor_stop_transmit_fn_t)stop_transmit,
	.write_byte = (sc_adaptor_write_byte_fn_t)write_byte,
};

static int start_transmit(struct sc_byte_adaptor_ch32v_hwi2c *self)
{
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
		;
	I2C_GenerateSTART(I2C1, ENABLE);

	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		;
	I2C_Send7bitAddress(I2C1, self->address, I2C_Direction_Transmitter);

	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;

	return 0;
}

static int stop_transmit(struct sc_byte_adaptor_ch32v_hwi2c *self)
{
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;
	I2C_GenerateSTOP(I2C1, ENABLE);
	return 0;
}

static int write_byte(struct sc_byte_adaptor_ch32v_hwi2c *self, int data)
{
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) == RESET)
		;
	I2C_SendData(I2C1, data);
	return 0;
}

int sc_byte_adaptor_ch32v_hwi2c_init(struct sc_byte_adaptor_ch32v_hwi2c *self, int address)
{
	GPIO_InitTypeDef gpio_init = {0};
	I2C_InitTypeDef i2c_init = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	/// When Remap is set, I2C pins will become PB8 and PB9.
	// GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);

	gpio_init.GPIO_Pin = GPIO_Pin_6;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init);

	gpio_init.GPIO_Pin = GPIO_Pin_7;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init);

	i2c_init.I2C_ClockSpeed = 400000;
	i2c_init.I2C_Mode = I2C_Mode_I2C;
	i2c_init.I2C_DutyCycle = I2C_DutyCycle_16_9;
	i2c_init.I2C_OwnAddress1 = 0x11;
	i2c_init.I2C_Ack = I2C_Ack_Enable;
	i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &i2c_init);

	I2C_Cmd(I2C1, ENABLE);

	// I2C_AcknowledgeConfig(I2C1, ENABLE);

	self->adaptor = &adaptor_interface;
	self->address = address << 1;
	return 0;
}

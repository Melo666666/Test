#ifndef MODULE_UTILS_H
#define MODULE_UTILS_H

#include "stm32f10x.h"

uint16_t modbus_crc_calc(uint8_t *pucFrame, uint16_t usLen);

#ifdef CONFIG_DEBUG
void config_assert_handler(const char *ex_string, const char *func, uint32_t line);
#endif

#endif

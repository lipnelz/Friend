#pragma once
#include <zephyr/kernel.h>

int settings_start(void);

bool settings_read_enable(void);
void settings_write_enable(bool enable);
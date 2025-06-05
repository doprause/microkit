#pragma once

#include "libs/microkit/lib/core.h"
#include "microkit/config/microkit.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_ASSERT)

void console_assertion(const char* message);
void gpio_assertion(void);

#endif // MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_ASSERT)

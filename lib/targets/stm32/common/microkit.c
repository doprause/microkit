#include "microkit/lib/contracts/platform/time.h"
#include "microkit/lib/microkit.h"

static void init(void) {
}

static void start(void) {
}

static void stop(void) {
}

MicrokitInterface Microkit = {
    .init = init,
    .start = start,
    .stop = stop,

#if defined(MICROKIT_USE_TIME)
    .time = {
        .delay = {
            .micros = microkit_time_delay_micros,
            .millis = microkit_time_delay_millis,
        }},
#endif
};

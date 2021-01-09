#ifndef MONKEY_H_
#define MONKEY_H_

#include <stdint.h>

extern "C" {
    extern const uint8_t MONKEY_PACKAGE[];
    extern int MONKEY_MONKEY_OFFSET;
    extern int MONKEY_MONKEY_SIZE;
}
#define MONKEY_MONKEY_DATA (MONKEY_PACKAGE + MONKEY_MONKEY_OFFSET)

#endif

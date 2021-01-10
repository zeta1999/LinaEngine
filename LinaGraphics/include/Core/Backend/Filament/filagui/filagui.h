#ifndef FILAGUI_H_
#define FILAGUI_H_

#include <stdint.h>

extern "C" {
    extern const uint8_t FILAGUI_PACKAGE[];
    extern int FILAGUI_UIBLIT_OFFSET;
    extern int FILAGUI_UIBLIT_SIZE;
}
#define FILAGUI_UIBLIT_DATA (FILAGUI_PACKAGE + FILAGUI_UIBLIT_OFFSET)

#endif

#include <stdint.h>
#include "FpgaConfig.h"

uint32_t char_to_seg_binary(char x);

void hex_write_uint(uint32_t x, EightBitPio* hexPair);
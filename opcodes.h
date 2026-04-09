
#ifndef GEN_OPCODES_H
#define GEN_OPCODES_H

#include <stddef.h>

#if TARGET == i8080 || TARGET == i8085
#define OPCODE_MAX_BYTES 3

#elif TARGET == z80 || TARGET == z80intel
#define OPCODE_MAX_BYTES 5

#else
#error "unknown TARGET, cannot determine OPCODE_MAX_BYTES"

#endif

extern const size_t OPCODE_MAX_STRLEN;

extern const size_t PRIMARY_LEN;
extern const char *PRIMARY_TABLE[];

#endif
/* end of file */

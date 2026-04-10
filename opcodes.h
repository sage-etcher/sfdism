
#ifndef GEN_OPCODES_H
#define GEN_OPCODES_H

#include <stddef.h>

#define i8080       0
#define i8085       1
#define z80         2
#define z80intel    3

#define OPCODE_MAX_BYTES 4

extern const size_t OPCODE_MAX_STRLEN;

extern const char  *PRIMARY_TABLE[];

#if FLAVOR == z80 || FLAVOR == z80intel
extern const char  *BIT_TABLE[];
extern const char  *MISC_TABLE[];
extern const char  *IX_TABLE[];
extern const char  *IX_BIT_TABLE[];
extern const char  *IY_TABLE[];
extern const char  *IY_BIT_TABLE[];
#endif

#endif
/* end of file */

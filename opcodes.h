
#ifndef GEN_OPCODES_H
#define GEN_OPCODES_H

#include <stddef.h>

#define i8080       0
#define i8085       1
#define z80         2
#define z80intel    3

#if FLAVOR == i8080 || FLAVOR == i8085
#define OPCODE_MAX_BYTES 3

#elif FLAVOR == z80 || FLAVOR == z80intel
#define OPCODE_MAX_BYTES 4

#else
#error "unknown FLAVOR, cannot determine OPCODE_MAX_BYTES"

#endif

extern const size_t OPCODE_MAX_STRLEN;

extern const size_t PRIMARY_LEN;
extern const char  *PRIMARY_TABLE[];

#if FLAVOR == z80 || FLAVOR == z80intel
extern const size_t BIT_LEN;
extern const char  *BIT_TABLE[];

extern const size_t MISC_LEN;
extern const char  *MISC_TABLE[];

extern const size_t IX_LEN;
extern const char  *IX_TABLE[];

extern const size_t IX_BIT_LEN;
extern const char  *IX_BIT_TABLE[];

extern const size_t IY_LEN;
extern const char  *IY_TABLE[];

extern const size_t IY_BIT_LEN;
extern const char  *IY_BIT_TABLE[];
#endif

#endif
/* end of file */

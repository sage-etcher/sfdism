
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "opcodes.h"

static FILE *s_fp = NULL;
static uint16_t s_offset = 0x0000;

static uint16_t s_opcode_addr = 0x0000;
static uint8_t s_opcode_bytes[OPCODE_MAX_BYTES];
static size_t s_opcode_bytes_cnt = 0;

static uint8_t getbyte (void);
static void dism_opcode (uint8_t opcode, const char *opcode_list[]);
static void dism_opcode_reverse (uint8_t data_byte, const char *opcode_list[]);

int
main (int argc, char **argv)
{
    int exitcode = 1;
    int rc = 0;
    char *filepath = NULL;
    uint8_t byte = 0x00;
    size_t i = 0;
    time_t now;

    if (argc <= 2)
    {
        printf ("usage: %s OFFSET BIN_FILE\n", argv[0]);
        return 1;
    }

    s_offset = strtoul(argv[1], NULL, 16);
    filepath = argv[2];
    s_fp = fopen (filepath, "rb");
    if (s_fp <= 0)
    {
        perror ("cannot open file");
        return 1;
    }

    (void)time (&now);
    printf ("; this file was generated using %s on %s", argv[0], ctime (&now));
    printf ("; using input binary file: %s\n", filepath);
    printf ("; and address offset: %04x\n", s_offset);

    for (;;)
    {
        s_opcode_addr = s_offset;
        s_opcode_bytes_cnt = 0;

        /* log asm opcode */
        byte = getbyte ();
        switch (byte)
        {
#if FLAVOR == z80 || FLAVOR == z80intel
        case 0xcb: /* bits */
            byte = getbyte ();
            dism_opcode (byte, BIT_TABLE);
            break;

        case 0xed: /* msic */
            byte = getbyte ();
            dism_opcode (byte, MISC_TABLE);
            break;

        case 0xdd: /* ix */
            byte = getbyte ();

            if (byte == 0xcb)   /* ix_bits */
            {
                byte = getbyte ();
                dism_opcode_reverse (byte, IX_BIT_TABLE);
            }
            else
            {
                dism_opcode (byte, IX_TABLE);
            }
            break;

        case 0xfd: /* iy */
            byte = getbyte ();
            if (byte == 0xcb)   /* iy_bits */
            {
                byte = getbyte ();
                dism_opcode_reverse (byte, IY_BIT_TABLE);
            }
            else
            {
                dism_opcode (byte, IY_TABLE);
            }
            break;
#endif

        default:
            dism_opcode (byte, PRIMARY_TABLE);
            break;

        }

        /* log offset */
        printf (";%04x\t", s_opcode_addr);

        /* log opcode bytes */
        for (i = 0; i < s_opcode_bytes_cnt; i++)
        {
            printf ("%02x ", s_opcode_bytes[i]);
        }
        while (++i <= OPCODE_MAX_BYTES)
        {
            printf ("   ");
        }

        /* log opcode chars */
        printf ("\t");
        for (i = 0; i < s_opcode_bytes_cnt; i++)
        {
            if (isprint (s_opcode_bytes[i]))
            {
                putchar (s_opcode_bytes[i]);
            }
            else
            {
                putchar ('.');
            }
        }
        while (++i <= OPCODE_MAX_BYTES)
        {
            putchar (' ');
        }


        printf ("\n");
    }
}

static uint8_t 
getbyte (void)
{
    int exitcode;
    uint8_t byte;
    size_t n = fread (&byte, 1, 1, s_fp);

    if (n != 1)
    {
        if (feof (s_fp))
        {
            exitcode = EXIT_SUCCESS;
        }
        else
        {
            exitcode = EXIT_FAILURE;
            errno = ferror (s_fp);
            perror ("failed to read byte from file");
        }
        fclose (s_fp);
        exit (exitcode);
    }

    if (s_opcode_bytes_cnt >= OPCODE_MAX_BYTES)
    {
        printf ("error s_opcode_bytes overflow\n");
        fclose (s_fp);
        exit (EXIT_FAILURE);
    }
    s_opcode_bytes[s_opcode_bytes_cnt++] = byte;
    s_offset++;

    return byte;
}

static char
xtoc (uint8_t x)
{
    x &= 0x0f;
    if (x >= 0xa)
    {
        return x + 'a' - 0x0a;
    }

    return x + '0';
}

static void
dism_opcode (uint8_t opcode_byte, const char *opcode_table[])
{
    const char *opcode = opcode_table[opcode_byte];
    const char *iter = opcode;
    char c;

    char buf[OPCODE_MAX_STRLEN+1];
    char *buf_iter = buf;

    uint8_t ibyte_low  = 0x00;
    uint8_t ibyte_high = 0x00;

    /* print opcode */
    while ((c = *iter++))
    {
        switch (c)
        {
        case '^':   /* relative jump */
            ibyte_low  = getbyte();

            buf_iter += sprintf (buf_iter, "%04x", 
                    s_opcode_addr + (int16_t)(int8_t)ibyte_low);
            break;

        case '%':   /* intermediate signed byte */
            ibyte_low  = getbyte();

            buf_iter += sprintf (buf_iter, "%+d", (int8_t)ibyte_low);
            break;

        case '!':   /* intermediate byte */
            ibyte_low  = getbyte();

            *buf_iter++ = xtoc (ibyte_low >> 4);
            *buf_iter++ = xtoc (ibyte_low);
            break;

        case '@':   /* intermediate word */
            ibyte_low  = getbyte ();
            ibyte_high = getbyte ();

            *buf_iter++ = xtoc (ibyte_high >> 4);
            *buf_iter++ = xtoc (ibyte_high);
            *buf_iter++ = xtoc (ibyte_low >> 4);
            *buf_iter++ = xtoc (ibyte_low);
            break;

        default:
            *buf_iter++ = c;
            break;
        }
    }

    *buf_iter = '\0';
    printf ("\t%-*s\t\t", (int)OPCODE_MAX_STRLEN, buf);
}

static void
dism_opcode_reverse (uint8_t data_byte, const char *opcode_list[])
{
    size_t bytes_cnt;
    uint8_t opcode = getbyte ();

    s_opcode_bytes_cnt -= 2;        /* dont overflow s_opcode_bytes  */
    bytes_cnt = s_opcode_bytes_cnt; /* backup the base count */
    fseek (s_fp, -2L, SEEK_CUR);    /* let dism_opcode read databyte */
    dism_opcode (opcode, opcode_list);

    fseek (s_fp, 1L, SEEK_CUR);     /* realign data stream for next opcode */

    s_opcode_bytes[bytes_cnt++] = data_byte;    /* set correct print order */
    s_opcode_bytes[bytes_cnt++] = opcode;
    s_opcode_bytes_cnt = bytes_cnt;             /* fix the base count */
}

/* end of file */

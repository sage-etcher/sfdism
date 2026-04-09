
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opcodes.h"

static FILE *s_fp = NULL;
static uint16_t s_offset = 0xc000;

static uint16_t s_opcode_addr = 0x0000;
static uint8_t s_opcode_bytes[OPCODE_MAX_BYTES];
static size_t s_opcode_bytes_cnt = 0;

static uint8_t getbyte (void);
static void dism_opcode (uint8_t opcode, const char *opcode_list[]);

int
main (int argc, char **argv)
{
    int exitcode = 1;
    int rc = 0;
    char *filepath = NULL;
    uint8_t byte = 0x00;
    size_t i = 0;

    filepath = argv[1];
    s_fp = fopen (filepath, "rb");
    if (s_fp <= 0)
    {
        perror ("cannot open file");
        return 1;
    }

    printf ("\torg     0%04xh\n", s_offset);
    for (;;)
    {
        s_opcode_addr = s_offset;
        s_opcode_bytes_cnt = 0;

        /* log asm opcode */
        byte = getbyte ();
        dism_opcode (byte, PRIMARY_TABLE);

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
        case '@':   /* intermediate word */
            ibyte_low  = getbyte ();
            ibyte_high = getbyte ();

            *buf_iter++ = xtoc (ibyte_high >> 4);
            *buf_iter++ = xtoc (ibyte_high);
            *buf_iter++ = xtoc (ibyte_low >> 4);
            *buf_iter++ = xtoc (ibyte_low);
            break;

        case '$':   /* intermediate byte */
            ibyte_low  = getbyte();

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

/* end of file */

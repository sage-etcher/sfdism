
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint8_t getbyte (void);

static FILE *s_fp = NULL;
static uint16_t s_offset = 0x0000;

static void dis_primary (uint8_t byte);
static void dis_cb (uint8_t byte);
static void dis_dd (uint8_t byte);
static void dis_ed (uint8_t byte);
static void dis_fd (uint8_t byte);

int
main (int argc, char **argv)
{
    int exitcode = 1;
    int rc = 0;
    char *filepath = NULL;
    uint8_t byte = 0x00;

    filepath = argv[0];
    s_fp = fopen (filepath, "rb");
    if (s_fp <= 0)
    {
        perror ("cannot open file");
        return 1;
    }

    for (;;)
    {
        printf ("%04x ", s_offset);

        byte = getbyte ();
        switch (byte)
        {
        case 0xcb: dis_cb (getbyte ()); break;
        case 0xdd: dis_dd (getbyte ()); break;
        case 0xed: dis_ed (getbyte ()); break;
        case 0xfd: dis_fd (getbyte ()); break;
        default: dis_primary (byte); break;
        }
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

    s_offset++;

    return 0;
}

static void
dis_primary (uint8_t byte)
{
}

static void
dis_cb (uint8_t byte)
{
}

static void
dis_dd (uint8_t byte)
{
}

static void
dis_ed (uint8_t byte)
{
}

static void
dis_fd (uint8_t byte)
{
}


/* end of file */


#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 0
get_max (start_position, end_position) -> (long, u8):
    set max_char = 0
    set max_position = 0
    restore start_position;
    for i = start_position until end_position:
        c = fgetch();
        if c > max_char:
            max_char = c;
            max_position = i;

    return (max_position, max_char)

decode_line (void) -> u8:
    const MAX_DIGITS = 2;
    define digits[MAX_DIGITS+1] = "";

    store start_position = ftell();
    loop to '\n' or EOF;
    store end_position = ftell();

    set prev_position = start_position;
    for i = range(MAX_DIGITS-1):
        digits[i], prev_position = get_max(prev_position, end_position-i);

    digits[MAX_DIGITS] = '\0';

    set value = atoi(digits);
    return value;

main (void) -> int:
    const INPUT_FILE = "test_input";
    set sum = 0;

    with line = fopen(INPUT_FILE, "r"):
        sum = sum + decode_line ();

    print ("sum: ", sum);

    return 0;

#endif


static uint64_t
atou64 (const char *str)
{
    const uint64_t BASE = 10;
    uint64_t num = 0;
    const char *iter = NULL;

    for (iter = str; *iter; iter++)
    {
        if (!isdigit (*iter))
        {
            return num;
        }
        num *= BASE;
        num += *iter - '0';
    }

    return num;
}


static long
get_max (FILE *fp, long start_position, long end_position, char *p_ret_max)
{
    char c = 0;
    char max_char = 0;
    long max_position = 0;
    long i = 0;

    fseek (fp, start_position, SEEK_SET);
    for (i = start_position; i < end_position; i++)
    {
        c = getc (fp);
        //printf ("c %ld: %c\n", i, c);
        if (c > max_char)
        {
            max_char = c;
            max_position = i;
        }
    }

    *p_ret_max = max_char;
    return max_position;
}

static int64_t
decode_line (FILE *fp)
{
    enum { MAX_DIGITS = 12 };
    char digits[MAX_DIGITS+1] = "";
    long start_position = 0;
    long end_position = 0;
    long prev_position = 0;
    int64_t value = 0;
    long i = 0;
    char c = 0;

    start_position = ftell (fp);
    while (((c = fgetc (fp)) != EOF) && (c != '\n')) {}
    end_position = ftell (fp);

    if (start_position == end_position) 
    {
        //printf ("exit condition: %c %ld %ld\n", c, start_position, end_position);
        return -1;
    }

    prev_position = start_position;
    for (i = 0; i < MAX_DIGITS; i++)
    {
        prev_position = get_max (fp, prev_position, end_position-MAX_DIGITS+i, &digits[i]) + 1;
        //printf ("digits[%ld]: %c\n", i, digits[i]);
    }

    (void)fgetc (fp);

    digits[MAX_DIGITS] = '\0';
    //printf ("digits: %s\n", digits);

    value = atou64 (digits);
    //printf ("value: %ld\n", value);

    return value;
}

int
main (void)
{
    const char *INPUT_FILE = "input";
    FILE *fp = NULL;
    int64_t value = 0;
    int64_t sum = 0;

    fp = fopen (INPUT_FILE, "r");
    assert (fp != NULL);

    do {
        value = decode_line (fp);
        if (value < 0) break;

        sum += value;
    } while (!feof (fp));

    printf ("sum: %ld\n", sum);

    return 0;
}

/* end of file */

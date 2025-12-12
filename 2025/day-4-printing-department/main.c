
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long
get_pos (long x, long y, long line_length)
{
    return y * (line_length+1) + x;
}

static int
fgetc_pos (FILE *fp, long pos)
{
    fseek (fp, pos, SEEK_SET);
    return fgetc (fp);
}

static int
check_pos (FILE *fp, long x, long y, long line_length, long line_count)
{
    char c = 0;

    if ((x < 0) || (x >= line_length)) return 0;
    if ((y < 0) || (y >= line_count)) return 0;

    if ('@' == fgetc_pos (fp, get_pos (x, y, line_length)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static int
is_accessble (FILE *fp, long x, long y, long line_length, long line_count)
{
    int total = 0;
    total += check_pos (fp, x-1, y-1, line_length, line_count);
    total += check_pos (fp, x-1, y,   line_length, line_count);
    total += check_pos (fp, x-1, y+1, line_length, line_count);
    total += check_pos (fp, x,   y-1, line_length, line_count);
    total += check_pos (fp, x,   y+1, line_length, line_count);
    total += check_pos (fp, x+1, y-1, line_length, line_count);
    total += check_pos (fp, x+1, y,   line_length, line_count);
    total += check_pos (fp, x+1, y+1, line_length, line_count);

    return total < 4;
}

int
main (void)
{
    const char *INPUT_FILE = "input";
    FILE *fp = NULL;
    char c = 0;
    long file_size = 0;

    long line_length = 0;
    long line_count = 0;

    long cur_position = 0;
    long cur_x = 0;
    long cur_y = 0;

    int sum = 0;


    fp = fopen (INPUT_FILE, "r");
    assert (fp != NULL);

    fseek (fp, 0L, SEEK_END);
    file_size = ftell (fp);

    fseek (fp, 0L, SEEK_SET);
    for (line_length = 0; (c = fgetc (fp)) != '\n'; line_length ++) {}

    assert (file_size > 0);
    assert (line_length > 0);
    assert (line_length != LONG_MAX);
    assert (file_size % (line_length+1) == 0);
    line_count = file_size / (line_length+1);

    fseek (fp, 0L, SEEK_END);
    for (cur_y = 0; cur_y < line_count; cur_y++)
    {
        for (cur_x = 0; cur_x < line_length; cur_x++)
        {
            c = fgetc_pos (fp, get_pos (cur_x, cur_y, line_length));
            assert ((c == '.') || (c == '@'));

            if (c == '.') 
            {
                fputc ('.', stdout);
                continue;
            }

            if (is_accessble (fp, cur_x, cur_y, line_length, line_count))
            {
                fputc ('x', stdout);
                sum++;
            }
            else
            {
                fputc ('@', stdout);
            }
        }
        fputc ('\n', stdout);
    }

    printf ("sum: %d\n", sum);

    fclose (fp);
    return 0;
}

/* end of file */

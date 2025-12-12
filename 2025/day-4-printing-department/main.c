
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct warehouse {
    char *m;
    size_t size;
    char **plot;
    size_t width;
    size_t height;
};

static char *
fdump (const char *filename)
{
    FILE *fp = NULL;
    char *contents = NULL;
    long size = 0;

    fp = fopen (filename, "r");
    assert (fp != NULL);

    fseek (fp, 0L, SEEK_END);
    size = ftell (fp);
    assert (size > 0);

    contents = malloc (size+1);
    assert (contents != NULL);

    fseek (fp, 0L, SEEK_SET);
    fread (contents, sizeof (char), size, fp);

    contents[size] = '\0';
    return contents;
}

static char **
strcsplit (char *str, char split_c, size_t *p_ret_n)
{
    char **arr = NULL;
    char *iter = NULL;
    size_t i = 0;
    size_t n = 1;

    assert (str != NULL);
    assert (p_ret_n != NULL);

    /* get length */
    for (iter = str; (iter = strchr (iter, split_c)) != NULL; n++, iter++) {}

    /* allocate room for return array */
    assert (n != 0);
    arr = malloc (sizeof (char *) * n);
    assert (arr != NULL);

    /* fill the return array */
    iter = str;
    i = 0;
    do {
        if (iter > str)
        {
            *iter++ = '\0';
        }
        arr[i++] = iter;

        iter = strchr (iter, split_c);
    } while (iter != NULL);

    *p_ret_n = n;
    return arr;
}

static int
check_pos (struct warehouse *self, int x, int y)
{
    char c = 0;

    if ((x < 0) || (x >= self->width)) return 0;
    if ((y < 0) || (y >= self->height)) return 0;
    if (&self->plot[y][x] >= self->m + self->size) return 0;

    if ('@' == self->plot[y][x])
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static int
is_accessable (struct warehouse *self, int x, int y)
{
    int total = 0;
    total += check_pos (self, x-1, y-1);
    total += check_pos (self, x-1, y  );
    total += check_pos (self, x-1, y+1);
    total += check_pos (self, x,   y-1);
    total += check_pos (self, x,   y+1);
    total += check_pos (self, x+1, y-1);
    total += check_pos (self, x+1, y  );
    total += check_pos (self, x+1, y+1);

    return total < 4;
}

int
remove_movable (struct warehouse *self)
{
    int moved = 0;
    int x = 0;
    int y = 0;

    for (y = 0; y < self->height; y++)
    {
        for (x = 0; x < self->width; x++)
        {
            if (&self->plot[y][x] >= self->m + self->size)
            {
                continue;
            }

            if (self->plot[y][x] != '@') 
            {
                putchar (self->plot[y][x]);
                continue;
            }

            if (is_accessable (self, x, y))
            {
                self->plot[y][x] = 'x';
                moved++;
            }
            putchar (self->plot[y][x]);
        }
        putchar ('\n');
    }

    return moved;
}

int
main (int argc, char **argv)
{
    assert (argc >= 2);
    assert (argv != NULL);
    assert (argv[1] != NULL);

    int moved = 0;
    int sum = 0;
    const char *const INPUT_FILE = argv[1];
    struct warehouse floor = { 0 };

    floor.m = fdump (INPUT_FILE);
    floor.size = strlen (floor.m);
    floor.plot = strcsplit (floor.m, '\n', &floor.height);
    floor.width = strlen (floor.plot[0]);

    do {
        moved = remove_movable (&floor);
        sum += moved;
    } while (moved != 0);

    printf ("sum: %d\n", sum);

    free (floor.plot);
    free (floor.m);
    return 0;
}


#if 0

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
#endif

/* end of file */

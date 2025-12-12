
#include "stb_ds.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint64_t x, y;
} ivec2_t;

typedef struct {
    uint64_t key;
    uint64_t value;
} hmitem_t;

int
main (int argc, char **argv)
{
    FILE *fp = NULL;

    ivec2_t range = { 0 };
    int rc = 0;

    hmitem_t *ids = NULL;

    int sum = 0;


    assert (argc >= 2);
    assert (argv != NULL);
    assert (argv[1] != NULL);
    fp = fopen (argv[1], "r");
    assert (fp != NULL);

    while (2 == (rc = fscanf (fp, "%llu-%llu\n", &range.x, &range.y)))
    {
        //printf ("range: %llu - %llu\n", range.x, range.y);
        for (; range.x <= range.y; range.x++)
        {
            //printf ("id: %llu\n", range.x);
            hmput (ids, range.x, range.x);
        }
    }

    sum = hmlenu (ids);
    printf ("sum: %d\n", sum);

    fclose (fp);
    return 0;
}

/* end of file */

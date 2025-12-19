
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint64_t x, y;
} ivec2_t;

#define MAX(x,y) (x < y ? y : x)
#define MIN(x,y) (x > y ? y : x)

static inline int
are_overlaping (ivec2_t a, ivec2_t b)
{
    return a.x <= b.y && b.x <= a.y;
}

int
cmp_cb_range (const ivec2_t *p_x, const ivec2_t *p_y)
{
    return p_x->x - p_y->x;
}

int
main (int argc, char **argv)
{
    FILE *fp = NULL;

    ivec2_t oh = { 0 };
    int rc = 0;
    size_t ii = 0;
    size_t jj = 0;
    ivec2_t *ranges = NULL;
    size_t range_cnt = 0;
    int64_t diff = 0;
    uint64_t max = 0;
    uint64_t min = 0;

    int sum = 0;

    assert (argc >= 2);
    assert (argv != NULL);
    assert (argv[1] != NULL);
    fp = fopen (argv[1], "r");
    assert (fp != NULL);

    do {
        rc = fscanf (fp, "%"SCNu64"-%"SCNu64"\n", &oh.x, &oh.y);
        range_cnt++;
    } while (rc == 2);
    range_cnt--;


    assert (range_cnt != 0);
    ranges = malloc (sizeof (ivec2_t) * range_cnt);
    assert (ranges != NULL);

    fseek (fp, 0L, SEEK_SET);
    for (ii = 0; ii < range_cnt; ii++)
    {
        rc = fscanf (fp, "%"SCNu64"-%"SCNu64"\n", &ranges[ii].x, &ranges[ii].y);
        assert (ranges[ii].x <= ranges[ii].y);
    }

    fclose (fp);
    fp = NULL;

    /* for every entry */
    for (ii = 0; ii < range_cnt; ii++)
    {
        for (jj = 0; jj < range_cnt; jj++)
        {
            if (ii == jj) continue;
            if (!are_overlaping (ranges[ii], ranges[jj])) continue;

            printf ("clean: %"SCNu64"-%"SCNu64": %"SCNu64"-%"SCNu64"\n", 
                    ranges[ii].x, ranges[ii].y,
                    ranges[jj].x, ranges[jj].y);

            ranges[ii].x = MIN (ranges[ii].x, ranges[jj].x);
            ranges[ii].y = MAX (ranges[ii].y, ranges[jj].y);

            if (range_cnt - jj  > 0)
            {
                memmove (ranges+jj, ranges+jj+1, (range_cnt - jj - 1) * sizeof (*ranges));
                if (ii > jj)
                {
                    ii--;
                }
                jj = -1;
            }
            range_cnt--;
        }
    }

    for (ii = 0; ii < range_cnt; ii++)
    {
        printf ("%"SCNu64"-%"SCNu64"\n", ranges[ii].x, ranges[ii].y);
        sum += ranges[ii].y - ranges[ii].x + 1;
    }

    printf ("sum: %d\n", sum);

    free (ranges);
    return 0;
}
/* end of file */


#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint64_t x, y;
} ivec2_t;

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

    int sum = 0;

    assert (argc >= 2);
    assert (argv != NULL);
    assert (argv[1] != NULL);
    fp = fopen (argv[1], "r");
    assert (fp != NULL);

    do {
        rc = fscanf (fp, "%llu-%llu\n", &oh.x, &oh.y);
        range_cnt++;
    } while (rc == 2);
    range_cnt--;


    assert (range_cnt != 0);
    ranges = malloc (sizeof (ivec2_t) * range_cnt);
    assert (ranges != NULL);

    fseek (fp, 0L, SEEK_SET);
    for (ii = 0; ii < range_cnt; ii++)
    {
        rc = fscanf (fp, "%llu-%llu\n", &ranges[ii].x, &ranges[ii].y);
    }

    fclose (fp);
    fp = NULL;

    for (ii = 0; ii < range_cnt; ii++)
    {
        ivec2_t *r0 = &ranges[ii];
        for (jj = 0; jj < range_cnt; jj++)
        {
            if (ii == jj) continue;

            ivec2_t *r1 = &ranges[jj];
            ivec2_t c = { 0 };

            if ((r1->x <= r0->x) && (r0->x <= r1->y))
            {
            }

            // 2-130
            // 14-18
            // if x1 < x2 && y1 > y2: remove 2
            // if 


        }
    }

    printf ("sum: %d\n", sum);


    free (ranges);
    return 0;
}
/* end of file */

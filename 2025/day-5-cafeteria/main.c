
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
    size_t i = 0;
    ivec2_t *ranges = NULL;
    size_t range_cnt = 0;
    
    uint64_t id = 0;
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
    for (i = 0; i < range_cnt; i++)
    {
        rc = fscanf (fp, "%llu-%llu\n", &ranges[i].x, &ranges[i].y);
    }

    
    while ((rc = fscanf (fp, "%llu\n", &id)) == 1) 
    {
        for (i = 0; i < range_cnt; i++)
        {
            if (id < ranges[i].x) continue;
            if (id > ranges[i].y) continue;

            sum++;
            break;
        }
    }

    printf ("sum: %d\n", sum);


    free (ranges);
    fclose (fp);
    return 0;
}
/* end of file */

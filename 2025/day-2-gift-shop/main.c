
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define U32_STRING_MAX 10
#define U64_STRING_MAX 20

static uint_fast8_t
u64toa (char *buf, uint64_t x)
{
    const uint64_t BASE = 10;
    uint64_t ii = x;
    uint_fast8_t jj = 0;
    uint_fast8_t cnt = 1;
    char c = '\0';

    assert (BASE != 0);
    assert (buf != NULL);

    for (; ii >= BASE; cnt++, ii /= BASE) {}
    assert (U64_STRING_MAX >= cnt);

    for (ii = x, jj = 0; jj <= cnt; jj++, ii /= BASE)
    {
        c = ii % BASE + '0';
        buf[cnt - jj - 1] = c;
    }

    buf[cnt] = '\0';

    return cnt;
}


static int
is_repeating (uint64_t x)
{
    char buf[U64_STRING_MAX+1];
    uint_fast8_t buf_cnt = 0;

    char cmp[U64_STRING_MAX/2+1];
    uint_fast8_t cmp_cnt = 0;

    int match = 0;
    uint_fast8_t i = 0;

    buf_cnt = u64toa (buf, x);

    cmp_cnt = buf_cnt / 2;
    memcpy (cmp, buf, cmp_cnt);
    cmp[cmp_cnt] = '\0';


#if 0
    /* part 1 */
    /* replace lower commented  */
    if (cmp_cnt == 0) return 0;
    if (buf_cnt % 2) return 0;
    return strncmp (buf+cmp_cnt, cmp, cmp_cnt) == 0;
#endif


    /* i miss read and thought we was looking for any repeating, not 
     * just repeating twise owo
     * maybe this will be usefull for part 2? idk */
    /* yoooooo i predicted correct lets goo i can reuse this hehehehehe */
    for (; cmp_cnt >= 1; cmp[--cmp_cnt] = '\0')
    {
        if (buf_cnt % cmp_cnt) continue;

        match = 1;
        for (i = cmp_cnt; i < buf_cnt; i += cmp_cnt)
        {
            if (strncmp (buf+i, cmp, cmp_cnt) != 0)
            {
                match = 0;
                //printf ("fail: %s: %s\n", buf, cmp);
                break;
            }
        }

        if (match)
        {
            //printf ("pass: %s: %s\n", buf, cmp);
            break;
        }
    }
 
    return match;
}


int
main (void)
{
    int rc = 0;
    FILE *fp = NULL;
    uint64_t range_lo = 0;
    uint64_t range_hi = 0;

    uint64_t i = 0;

    uint64_t sum = 0;

    fp = fopen ("input", "r");
    if (fp == NULL)
    {
        perror ("cannot open input file");
        return 1;
    }

    do {
        rc = fscanf (fp, "%lu-%lu,", &range_lo, &range_hi);
        if (rc == EOF) break;

        for (i = range_lo; i <= range_hi; i++)
        {
            if (is_repeating (i))
            {
                sum += i;
            }
        }
    } while (!feof (fp));

    printf ("sum: %lu\n", sum);

    fclose (fp);
    return 0;
}




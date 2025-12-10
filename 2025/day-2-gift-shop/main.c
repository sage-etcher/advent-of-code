
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define U32_STRING_MAX 10

static uint_fast8_t
u32toa (char *buf, uint32_t x)
{
    const uint32_t BASE = 10;
    uint32_t ii = x;
    uint_fast8_t jj = 0;
    uint_fast8_t cnt = 1;
    char c = '\0';

    assert (BASE != 0);
    assert (buf != NULL);

    for (; ii >= BASE; cnt++, ii /= BASE) {}
    assert (U32_STRING_MAX >= cnt);

    for (ii = x, jj = 0; jj <= cnt; jj++, ii /= BASE)
    {
        c = ii % BASE + '0';
        buf[cnt - jj - 1] = c;
    }

    buf[cnt] = '\0';

    return cnt;
}


static int
is_repeating (uint32_t x)
{
    char buf[U32_STRING_MAX+1];
    uint_fast8_t buf_cnt = 0;

    char cmp[U32_STRING_MAX/2+1];
    uint_fast8_t cmp_cnt = 0;

    int match = 0;
    uint_fast8_t i = 0;

    buf_cnt = u32toa (buf, x);

    cmp_cnt = buf_cnt / 2;
    memcpy (cmp, buf, cmp_cnt);
    cmp[cmp_cnt] = '\0';

    for (; cmp_cnt >= 1; cmp[--cmp_cnt] = '\0')
    {
        if (buf_cnt % cmp_cnt) continue;

        match = 1;
        for (i = cmp_cnt; i < buf_cnt; i += cmp_cnt)
        {
            if (strncmp (buf+i, cmp, cmp_cnt) != 0)
            {
                match = 0;
                break;
            }
        }

        if (match)
        {
            printf ("match %s: %s\n", buf, cmp);
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
    uint32_t range_lo = 0;
    uint32_t range_hi = 0;

    uint32_t i = 0;

    uint32_t sum = 0;

    fp = fopen ("input", "r");
    if (fp == NULL)
    {
        perror ("cannot open input file");
        return 1;
    }

    do {
        rc = fscanf (fp, "%d-%d,", &range_lo, &range_hi);
        if (rc == EOF) break;

        for (i = range_lo; i <= range_hi; i++)
        {
            if (is_repeating (i))
            {
                sum += i;
            }
        }
    } while (!feof (fp));

    printf ("sum: %u\n", sum);

    fclose (fp);
    return 0;
}




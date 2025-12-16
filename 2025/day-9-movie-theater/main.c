
#include "ivec.h"

#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline uint64_t
abs64 (int64_t x)
{
    return (x < 0 ? -x : x);
}

int
main (int argc, char **argv)
{
    char *file_input = NULL;
    FILE *fp = NULL;
    int c = 0;

    size_t line_count = 0;
    u32vec2_t point = { .x = 0, .y = 0 };
    u32vec2_t *points = NULL;
    u32vec2_t *iter = NULL;

    size_t ii = 0;
    size_t jj = 0;
    uint64_t width = 0;
    uint64_t height = 0;
    uint64_t area = 0;
    uint64_t max_area = 0;
    u32vec2_t max_points[2] = { 0 };

    assert (argc >= 2);
    assert (argv != NULL);
    assert (argv[1] != NULL);

    file_input = argv[1];
    fp = fopen (file_input, "r");
    assert (fp != NULL);

    while ((c = fgetc (fp)) != EOF) if (c == '\n') line_count++;
    assert (line_count >= 1);
    line_count--;

    points = malloc (line_count * sizeof (*points));
    assert (points != NULL);

    iter = points;
    fseek (fp, 0L, SEEK_SET);
    while (2 == fscanf (fp, "%"SCNu32",%"SCNu32"\n", &point.x, &point.y))
    {
        *iter++ = point;
    }

    fclose (fp);

    for (ii = 0; ii < line_count; ii++)
    {
        for (jj = 0; jj < line_count; jj++)
        {
            width  = abs64((int64_t)points[ii].x - points[jj].x) + 1;
            height = abs64((int64_t)points[ii].y - points[jj].y) + 1;
            area = width * height;

            if (area <= max_area) continue;

            printf ("(%6"SCNu32", %6"SCNu32"): (%6"SCNu32", %6"SCNu32"): "
                    "%6"SCNu64"x%-6"SCNu64": %10"SCNu64"\n",
                    points[ii].x, points[ii].y, points[jj].x, points[jj].y,
                    width, height, area);
            max_area = area;
            max_points[0] = points[ii];
            max_points[1] = points[jj];
        }
    }

    printf ("max_area: %"SCNu64"\n", max_area);

    free (points);
    return 0;
}

/* end of file */

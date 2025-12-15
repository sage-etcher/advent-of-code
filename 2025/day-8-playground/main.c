
#include <stb_ds.h>

#include <assert.h>
#include <float.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint64_t x, y, z; } u64vec3_t;
typedef struct { uint64_t x, y; } u64vec2_t;

#define MAX_CONNECTIONS 10
#define MAX_CIRCUTS 1000

typedef struct junction_box {
    u64vec3_t point;
    int connections[MAX_CONNECTIONS];
    size_t connection_count;
} junction_box_t;

typedef u64vec2_t pair_t;

typedef struct {
    int key;
    int value;
} index_map_t;

typedef struct {
    index_map_t *map;
} circut_t;

static double
euclidean_distance (u64vec3_t a, u64vec3_t b)
{
    uint64_t x = a.x - b.x;
    uint64_t y = a.y - b.y;
    uint64_t z = a.z - b.z;
    return sqrt (x * x + y * y + z * z);
}

static junction_box_t *
get_junction_boxes (char *filename, size_t *p_ret_n)
{
    size_t i = 0;

    FILE *fp = NULL;
    int c = 0;

    junction_box_t *buf = NULL;
    size_t n = 0;

    assert (filename != NULL);
    assert (p_ret_n != NULL);

    fp = fopen (filename, "r");
    assert (fp != NULL);

    /* get number of points */
    while ((c = fgetc (fp)) != EOF) if (c == '\n') n++;

    /* get poitns */
    assert (n != 0);
    buf = malloc (n * sizeof (*buf));
    assert (buf != NULL);

    fseek (fp, 0L, SEEK_SET);
    for (; i < n; i++)
    {
        (void)fscanf (fp, "%"SCNu64",%"SCNu64",%"SCNu64"\n",
                &buf[i].point.x, &buf[i].point.y, &buf[i].point.z);
    }

    fclose (fp);

    *p_ret_n = n;
    return buf;
}

int
add_connection (junction_box_t *self, int index)
{
    assert (self != NULL);
    assert (self->connection_count <= MAX_CONNECTIONS);

    self->connections[self->connection_count++] = index;

    return 0;
}

int
are_directly_connected (junction_box_t *boxes, size_t n, int a, int b)
{
    size_t i = 0;
    junction_box_t *box_a = NULL;

    assert (boxes != NULL);
    assert (a < n);
    assert (b < n);

    box_a = &boxes[a];
    for (; i < box_a->connection_count; i++)
    {
        if (box_a->connections[i] == b) return 1;
    }

    return 0;
}

int
get_nearest (junction_box_t *boxes, size_t n, size_t cmp_index, double *p_dist)
{
    size_t i = 0;
    double distance = 0.0f;
    double min_distance = DBL_MAX;

    int min_index = -1;

    for (; i < n; i++)
    {
        if (i == cmp_index) continue;   /* dont match itself */

        /* dont match 2 points that are already connected */
        if (are_directly_connected (boxes, n, cmp_index, i)) continue;
        if (are_directly_connected (boxes, n, i, cmp_index)) continue;

        distance = euclidean_distance (boxes[cmp_index].point, boxes[i].point);
        if (distance >= min_distance) continue;

        min_distance = distance;
        min_index = i;
    }

    if (p_dist != NULL) *p_dist = min_distance;

    return min_index;
}

pair_t
get_next_nearest (junction_box_t *boxes, size_t n)
{
    size_t i = 0;

    int nearest_index = -1;

    double distance = 0.0f;
    double min_distance = DBL_MAX;

    pair_t nearest_pair = { .x= 0, .y = 0 };

    for (; i < n; i++)
    {
        nearest_index = get_nearest (boxes, n, i, &distance);
        if (distance < min_distance)
        {
            min_distance = distance;
            nearest_pair = (pair_t){ .x = i, .y = nearest_index };
        }
    }

    return nearest_pair;
}

void
make_circut_r (circut_t *self, junction_box_t *boxes, size_t n, size_t index)
{
    int i = 0;
    int leaf_index = 0;

    assert (self != NULL);
    assert (boxes != NULL);
    assert (index < n);

    hmput (self->map, index, index);
    for (i = 0; i < boxes[index].connection_count; i++)
    {
        leaf_index = boxes[index].connections[i];
        if (hmgeti (self->map, leaf_index) != -1) continue; /* value was already processed */

        make_circut_r (self, boxes, n, leaf_index);
    }
}

circut_t
make_circut (junction_box_t *boxes, size_t n, size_t index)
{
    circut_t circut = { .map = NULL };
    make_circut_r (&circut, boxes, n, index);
    return circut;
}


int
main (int argc, char **argv)
{
    char *input_file = NULL;

    size_t i = 0;
    size_t j = 0;
    size_t box_cnt = 0;
    junction_box_t *boxes = NULL;
    circut_t circuts[MAX_CIRCUTS] = { 0 };
    size_t circut_cnt = 0;
    size_t circut_length = 0;
    size_t top_circuts[3] = { 0, 0, 0 };
    size_t result = 1;

    pair_t nearest_pair = { .x = 0, .y = 0 };

    assert (argc >= 2);
    assert (argv != NULL);
    assert (argv[1] != NULL);
    input_file = argv[1];

    boxes = get_junction_boxes (input_file, &box_cnt);
    for (i = 0; i < 1000; i++)
    {
        nearest_pair = get_next_nearest (boxes, box_cnt);
        assert (nearest_pair.x != nearest_pair.y);

        add_connection (&boxes[nearest_pair.x], nearest_pair.y);
        add_connection (&boxes[nearest_pair.y], nearest_pair.x);
    }

    for (i = 0; i < box_cnt; i++)
    {
        for (j = 0; j < circut_cnt; j++)
        {
            if (hmgeti (circuts[j].map, i) != -1) goto element_handled;
        }

        assert (circut_cnt < MAX_CIRCUTS);
        circuts[circut_cnt++] = make_circut (boxes, box_cnt, i);

element_handled:
        continue;
    }

    for (i = 0; i < circut_cnt; i++)
    {
        circut_length = hmlen (circuts[i].map);
        printf ("%zu %zu\n", i, circut_length);
        for (j = 3; j-- > 0;)
        {
            if (top_circuts[j] < circut_length)
            {
                memmove (top_circuts, top_circuts+1, j*sizeof (*top_circuts));
                top_circuts[j] = circut_length;
                break;
            }
        }
    }

    for (i = 0; i < 3; i++)
    {
        result *= top_circuts[i];
    }

    printf ("result: %zu\n", result);

    free (boxes); boxes = NULL; box_cnt = 0;
    return 0;
}

/* end of file */

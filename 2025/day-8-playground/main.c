
#include "ivec.h"

#include <stb_ds.h>

#include <assert.h>
#include <float.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONNECTIONS 20
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
    const uint64_t x = a.x - b.x;
    const uint64_t y = a.y - b.y;
    const uint64_t z = a.z - b.z;
    //return sqrt (x * x + y * y + z * z);
    return x * x + y * y + z * z;
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

static int
native_bsearch_int_cb (const int *p_a, const int *p_b)
{
    return *p_a - *p_b;
}

static int
bsearch_nearest (int key, int *arr, size_t n)
{
    int lo = 0;
    int hi = n;
    int mid = 0;

    while (lo <= hi)
    {
        mid = (hi + lo) / 2;
        if (arr[mid] == key) 
        {
            return 0;
        }
        else if (arr[mid] <  key) 
        {
            lo = mid + 1;
        }
        else 
        {
            hi = mid - 1;
        }
    }

    return mid;
}

static int
binsert (int key, int *arr, size_t n)
{
    int i = 0;
    int mid = bsearch_nearest (key, arr, n);

    /*
    printf ("binsert: %2d %2d [ ", key, mid);
    for (i = 0; i < n; i++)
    {
        printf ("%2d ", arr[i]);
    }
    printf ("] -> [ ");
    */

    memmove (arr+mid+1, arr+mid, (n-mid) * sizeof (*arr));
    arr[mid] = key;

    /*
    for (i = 0; i < n+1; i++)
    {
        printf ("%2d ", arr[i]);
    }
    printf ("]\n");
    */

    return mid;
}

int
add_connection (junction_box_t *self, int index)
{
    int i = 0;
    assert (self != NULL);
    assert (self->connection_count <= MAX_CONNECTIONS);

    i = binsert (index, self->connections, self->connection_count);
    self->connection_count++;

    return 0;
}

int
are_directly_connected (junction_box_t *boxes, size_t n, int a, int b)
{
    junction_box_t *box_a = NULL;
    int *match = 0;

    assert (boxes != NULL);
    assert (a < n);
    assert (b < n);

    box_a = &boxes[a];
    match = bsearch (&b, box_a->connections, box_a->connection_count,
            sizeof (*box_a->connections), (__compar_fn_t)native_bsearch_int_cb);

    return (match != NULL);
}

pair_t
get_next_nearest_fast (junction_box_t *boxes, size_t n)
{
    size_t ii = 0;
    size_t jj = 0;

    double distance = 0.0f;
    double min_distance = DBL_MAX;

    pair_t nearest_pair = { .x = 0, .y = 0 };

    for (; ii < n; ii++)
    {
        for (jj = 0; jj < n; jj++)
        {
            if (ii == jj) continue;   /* dont match itself */

            /* dont match 2 points that are already connected */
            if (are_directly_connected (boxes, n, ii, jj)) continue;
            if (are_directly_connected (boxes, n, jj, ii)) continue;

            distance = euclidean_distance (boxes[ii].point, boxes[jj].point);
            if (distance > min_distance) continue;

            min_distance = distance;
            nearest_pair = (pair_t){ .x = ii, .y = jj };
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
    int max_connections = 0;

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

    assert (argc >= 3);
    assert (argv != NULL);
    assert (argv[1] != NULL);
    assert (argv[2] != NULL);
    input_file = argv[1];
    max_connections = atoi (argv[2]);
    assert (max_connections >= 0);

    boxes = get_junction_boxes (input_file, &box_cnt);
    for (i = 0; i < max_connections; i++)
    {
        nearest_pair = get_next_nearest_fast (boxes, box_cnt);
        if (nearest_pair.x == nearest_pair.y)
        {
            fprintf (stderr, "error: no more boxes can be connected\n");
            break;
        }

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
        printf ("%02zu: %3zu: [ ", circut_length, i);
        for (j = 0; j < circut_length; j++)
        {
            printf ("%4d ", circuts[i].map[j].value);
        }
        printf ("]\n");
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

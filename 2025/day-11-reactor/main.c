
#include <stb_ds.h>

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define OUTPUT_MAX 24
#define ID_SIZE     3

typedef char node_name_t[ID_SIZE+1];

typedef struct {
    node_name_t name;
    node_name_t outputs[OUTPUT_MAX];
    uint_fast8_t output_cnt;
} node_t;

typedef struct {
    char *key;
    node_t *value;
} node_map_t;


node_map_t *
get_nodes (char *filename)
{
    int rc = 0;

    FILE *fp = NULL;
    int c = 0;

    node_t *node = NULL;
    char *p_output = NULL;
    node_map_t *map = NULL;

    fp = fopen (filename, "r");
    assert (fp != NULL);

    do {
        node = malloc (sizeof (node_t));
        assert (node != NULL);

        /* extract line from file */
        node->output_cnt = 0;

        rc = fread (node->name, sizeof (char), ID_SIZE, fp);
        node->name[ID_SIZE] = '\0';

        if (rc != (ID_SIZE * sizeof (char))) break;

        c = fgetc (fp); assert (c == ':');
        while ((c = fgetc (fp)) == ' ')
        {
            p_output = (char *)&node->outputs[node->output_cnt++];

            (void)fread (p_output, sizeof (char), ID_SIZE, fp);
            p_output[ID_SIZE] = '\0';
        }

        assert (c == '\n');

        /* process line */
        (void)shput (map, node->name, node);

    } while (c != EOF);

    fclose (fp);
    return map;
}

int
count_lines (node_map_t *map, char *node, int sum)
{
    uint_fast8_t i = 0;
    int match_index = 0;

    node_t *p_node = NULL;
    char *p_node_name = NULL;

    assert (map != NULL);

    //printf ("node: %s\n", node);
    match_index = shgeti (map, node);
    assert (match_index != -1);
    p_node = map[match_index].value;

    for (i = 0; i < p_node->output_cnt; i++)
    {
        p_node_name = p_node->outputs[i];
        if (strncmp ("out", p_node_name, ID_SIZE) == 0)
        {
            sum++;
        }
        else
        {
            sum = count_lines (map, p_node_name, sum);
        }
    }

    return sum;
}

int
main (int argc, char **argv)
{
    int rc = 0;
    char *file_input = NULL;
    node_map_t *map = NULL;
    int sum = 0;


    assert (argc <= 2);
    assert (argv != NULL);
    assert (argv[1] != NULL);

    file_input = argv[1];
    map = get_nodes (file_input);
    sum = count_lines (map, "you", 0);

    printf ("sum: %d\n", sum);

    return 0;
}

/* end of file */

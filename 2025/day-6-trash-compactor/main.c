
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    ACTION_MULT,
    ACTION_ADD,
};


int
main (int argc, char **argv)
{
    char *file_input = NULL;
    FILE *fp = NULL;

    size_t alloc = 0;
    size_t count = 0;
    int *actions = NULL;
    uint64_t *totals = NULL;

    size_t new_alloc = 0;
    void *tmp = NULL;

    int c = 0;
    int i = 0;
    int rc = 0;
    uint64_t num = 0;

    uint64_t sum = 0;


    assert (argc >= 2);
    assert (argv != NULL);
    assert (argv[1] != NULL);

    file_input = argv[1];
    fp = fopen (file_input, "r");
    assert (fp != NULL);

    while ((c = fgetc (fp)) != EOF)
    {
        if (count >= alloc)
        {
            new_alloc = (alloc ? alloc * 2 : 2);
            tmp = realloc (actions, sizeof (*actions) * new_alloc);
            assert (tmp != NULL);
            actions = tmp;
            alloc = new_alloc;
        }

        switch (c)
        {
        case '*': actions[count++] = ACTION_MULT; break;
        case '+': actions[count++] = ACTION_ADD; break;
        default: continue;
        }
    }
    printf ("count %zu alloc %zu\n", count, alloc);
    for (i = 0; i < count; i++) printf ("actions[%d]: %d\n", i, actions[i]);

    assert (count != 0);
    totals = calloc (count, sizeof (*totals));
    assert (totals != NULL);

    for (i = 0; i < count; i++)
    {
        if (actions[i] == ACTION_MULT)
        {
            totals[i] = 1;
        }
    }

    i = 0;
    fseek (fp, 0L, SEEK_SET);
    while ((rc = fscanf (fp, "%"SCNu64" ", &num)) == 1)
    {
        switch (actions[i])
        {
        case ACTION_ADD:  totals[i] += num; break;
        case ACTION_MULT: totals[i] *= num; break;
        default:
            fprintf (stderr, "unknown action\n");
            abort ();
            break;
        }

        i++;
        i %= count;
    }

    sum = 0;
    for (i = 0; i < count; i++)
    {
        sum += totals[i];
    }

    printf ("totals: %"SCNu64"\n", sum);

    (void)fclose (fp);
    return 0;
}

/* end of file */


#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int
main (int argc, char **argv)
{
    #define LINE_PREV (line_buffer)
    #define LINE_CUR  (line_buffer+line_length)

    int rc = 0;
    int first_iteration;

    char *input_file = NULL;
    FILE *fp = NULL;
    int c = 0;

    size_t line_length = 0;
    char *line_buffer = NULL;

    size_t i = 0;

    int sum = 0;

    assert (argc >= 2);
    assert (argv != NULL);
    assert (argv[1] != NULL);

    input_file = argv[1];
    fp = fopen (input_file, "r");
    assert (fp != NULL);

    for (line_length = 0; (c = fgetc (fp)) != '\n'; line_length++);
    assert (line_length != 0);

    line_buffer = malloc (line_length * 2);
    assert (line_buffer != NULL);

    fseek (fp, 0L, SEEK_SET);
    (void)fread (LINE_PREV, sizeof (char), line_length, fp);
    (void)fgetc (fp); /* pop newline */

    while (line_length == fread (LINE_CUR, sizeof (char), line_length, fp))
    {
        for (i = 0; i < line_length; i++)
        {
            if ((LINE_PREV[i] != '|') && (LINE_PREV[i] != 'S')) continue;

            switch (LINE_CUR[i])
            {
            case '.': LINE_CUR[i] = '|'; break;
            case '^': 
                LINE_CUR[i-1] = '|';
                LINE_CUR[i+1] = '|';
                sum++;
                break;
            }
        }
        printf ("line: %.*s\n", (int)line_length, LINE_CUR);

        (void)memcpy (LINE_PREV, LINE_CUR, line_length); /* swap buffers */
        (void)fgetc (fp); /* pop newline */
    }

    printf ("sum: %d\n", sum);

    free (line_buffer);
    fclose (fp);
    return 0;
}

/* end of file */

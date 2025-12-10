
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main (void)
{
    FILE *fp = NULL;

    char direction = '\0';
    uint32_t magnitude = 0;

    int32_t movement = 0;
    int32_t position = 50;

    uint32_t real_code = 0;

    /* open the input file */
    fp = fopen ("input", "r");

    /* loop across every line of data */
    do {
        fscanf (fp, "%c%u\n", &direction, &magnitude);

        movement = magnitude;
        if (direction == 'L')
        {
            movement *= -1;
        }

        /* check if at 0 */
        position += movement;
        if (!(position % 100))
        {
            real_code++;
        }
    } while (!feof (fp));

    printf ("real_code: %u\n", real_code);

    fclose (fp);
    exit (EXIT_SUCCESS);
}

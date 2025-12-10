
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))

int
main (void)
{
    FILE *fp = NULL;

    char direction = '\0';
    uint32_t magnitude = 0;

    int32_t movement = 0;

    int32_t a = 0;
    int32_t b = 0;
    int32_t position = 50;
    int32_t new_position = 0;

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

        /* check if at, or past, 0 */
        a = position + movement;
        b = ABS (a);

        real_code += b / 100;
        new_position = b % 100;

        if ((a <= 0) && (position > 0))
        {
            real_code++;
        }

        if ((a < 0) && (new_position != 0))
        {
            new_position = 100 - new_position;
        }

        position = new_position;

        printf ("%c%-3d %4d %2d %2d\n", direction, magnitude, movement, position, real_code);

    } while (!feof (fp));

    printf ("real_code: %u\n", real_code);

    fclose (fp);
    exit (EXIT_SUCCESS);
}

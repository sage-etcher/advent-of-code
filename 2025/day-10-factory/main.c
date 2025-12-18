
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define LED_DIAG_MAX 10
#define LED_ON  '#'
#define LED_OFF '.'

#define BTN_MAX 15

typedef struct {
    uint_fast8_t cnt;
    uint_fast8_t m[LED_DIAG_MAX];
    int press;
} button_t;

typedef struct {
    uint_fast8_t led_cnt;
    char leds[LED_DIAG_MAX];

    uint_fast8_t btn_cnt;
    button_t btns[BTN_MAX];
} machine_t;

static void
machine_reset (machine_t *self)
{
    assert (self != NULL);
    assert (self->led_cnt <= LED_DIAG_MAX);

    (void)memset (self->leds, LED_OFF, self->led_cnt);
    self->led_cnt = 0;
    self->btn_cnt = 0;
}

static void
machine_init (machine_t *self, uint_fast8_t led_cnt)
{
    assert (self != NULL);
    assert (led_cnt <= LED_DIAG_MAX);

    self->led_cnt = 0;
    self->btn_cnt = 0;
    (void)machine_reset (self);

    self->led_cnt = led_cnt;
}

static void
machine_btn_add (machine_t *self, button_t btn)
{
    assert (self != NULL);
    assert (self->btn_cnt < BTN_MAX);
    assert (btn.cnt <= self->led_cnt);

    self->btns[self->btn_cnt++] = btn;
}

static void
machine_press_buttons (machine_t *self)
{
    uint_fast8_t ii = 0;
    int jj = 0;

    button_t *p_btn = NULL;
    char *p_led = NULL;

    assert (self != NULL);

    (void)memset (self->leds, LED_OFF, self->led_cnt);

    for (ii = 0; ii < self->btn_cnt; ii++)
    {
        p_btn = &self->btns[ii];

        if (!p_btn->press) continue;

        for (jj = 0; jj < p_btn->cnt; jj++)
        {
            assert (p_btn->cnt < LED_DIAG_MAX);
            assert (p_btn->m[jj] < LED_DIAG_MAX);

            p_led = &self->leds[p_btn->m[jj]];
            *p_led = (*p_led == LED_ON ? LED_OFF : LED_ON);
        }
    }
}

static int
machine_bruteforce (machine_t *self, char *match, int iterations)
{
    int ii = 0;
    int jj = 0;

    for (ii = 0; ii < self->btn_cnt; ii++)
    {
        assert (iterations > 0);

        //machine_btn_push (self, ii);
        self->btns[ii].press ^= 1;

        if (iterations == 1)
        {
            machine_press_buttons (self);

            /* log */
            /*
            printf ("[%.*s] ", self->led_cnt, self->leds);
            for (jj = 0; jj < self->btn_cnt; jj++)
            {
                printf ("%d:%d ", jj, self->btns[jj].press);
            }
            printf ("\n");
            */

            /* test */
            if (memcmp (match, self->leds, self->led_cnt) == 0)
            {
                return 1;
            }
            else
            {
                goto loop_continue;
            }
        }

        if (machine_bruteforce (self, match, iterations - 1))
        {
            return 1;
        }

loop_continue:
        self->btns[ii].press ^= 1;
    }

    return 0;
}

int
main (int argc, char **argv)
{
    char *file_input = NULL;
    FILE *fp = NULL;
    int c = 0;

    uint_fast8_t max_iterations = 0;
    int i = 0;

    int sum = 0;

    char led_match[LED_DIAG_MAX];
    uint_fast8_t led_cnt = 0;
    machine_t self = { 0 };
    button_t btn = { 0 };

    assert (argc >= 2);
    assert (argv != NULL);
    assert (argv[1] != NULL);

    file_input = argv[1];
    fp = fopen (file_input, "r");
    assert (fp != NULL);

    while ((c = fgetc (fp)) != EOF)
    {
        if (c != '[') continue;

        self.btn_cnt = 0;
        self.led_cnt = 0;

        led_cnt = 0;
        while (((c = fgetc (fp)) == LED_ON) || ( c == LED_OFF))
        {
            led_match[led_cnt] = c;
            led_cnt++;
        }
        self.led_cnt = led_cnt;
        assert (c == ']');
        assert (led_cnt > 0);

        c = fgetc (fp);
        assert (c == ' ');
        while ((c = fgetc (fp)) == '(')
        {
            btn.cnt = 0;
            while (isdigit (c = fgetc (fp)))
            {
                btn.m[btn.cnt] = c - '0';
                btn.cnt++;

                c = fgetc (fp);
                if (c == ',') continue;
            }
            machine_btn_add (&self, btn);
            assert (c == ' ');
        }
        assert (c == '{');

        while ((c = fgetc (fp)) != '\n') {}

        max_iterations = self.led_cnt;
        for (i = 1; i < max_iterations; i++)
        {
            if (machine_bruteforce (&self, led_match, i))
            {
                printf ("match found in %d presses\n", i);
                break;
            }
        }

        if (i >= max_iterations)
        {
            fprintf (stderr, "error: no match found in %d iterations\n", i);
            return 1;
        }

        sum += i;
    }

    printf ("sum: %d\n", sum);
    return 0;
}


#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

void toggle_leds()
{
    for (int i = 0; i < LEDS_NUMBER; i++)
    {
        bsp_board_led_invert(i);
        nrf_delay_ms(50);
    }
}

int main(void)
{
    bsp_board_init(BSP_INIT_LEDS);

    while (true)
    {
        toggle_leds();
    }
}

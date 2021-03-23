
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "cache.h"


void toggle_leds()
{
    for (int i = 0; i < LEDS_NUMBER; i++)
    {
        bsp_board_led_invert(i);
        nrf_delay_ms(500);
    }
}

void blink(int delay)
{
    // Leds on
    for (int i = 0; i < LEDS_NUMBER; i++)
        bsp_board_led_on(i);

    nrf_delay_ms(delay);

    // Leds off
    for (int i = 0; i < LEDS_NUMBER; i++)
        bsp_board_led_off(i);

    nrf_delay_ms(delay);
}

int main(void)
{
    const int blink_delay = 100;

    bsp_board_init(BSP_INIT_LEDS);
    cache_enable();

    volatile int tmp = cache_hit();
    cache_disable();

    cache_hit_miss_reset();

    while (true)
    {
        blink(blink_delay);
    }
}

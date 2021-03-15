
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

void cache_enable()
{
    const int enable_bit = (1 << 0);
    const int profiler_enable_bit = (1 << 8);

    NRF_NVMC->ICACHECNF = enable_bit | profiler_enable_bit;
}

void cache_disable()
{
    NRF_NVMC->ICACHECNF = 0x0;
}

int cache_hit()
{
    return NRF_NVMC->IHIT;
}

int cache_miss()
{
    return NRF_NVMC->IMISS;
}

void cache_hit_miss_reset()
{
    NRF_NVMC->IHIT  = 0x0;
    NRF_NVMC->IMISS = 0x0;
}

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
    cache_enable();

    cache_disable();

    cache_hit_miss_reset();

    if (cache_hit() > 0) {
        while(true) 
            toggle_leds();
    }

    while (true)
    {
        toggle_leds();
    }
}

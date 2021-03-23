#include "cache.h"
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
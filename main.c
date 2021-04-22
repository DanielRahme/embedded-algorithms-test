
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "cache.h"

// FFT stuff
#include "arm_math.h"
#include "arm_const_structs.h"

#define RUN_FROM_RAM 1
#define RUN_WITH_CACHE 0

#define TEST_LENGTH_SAMPLES 2048
extern float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES];
static float32_t testOutput[TEST_LENGTH_SAMPLES/2];

// Example from: 
// https://github.com/ARM-software/CMSIS/blob/master/CMSIS/DSP_Lib/Examples/arm_fft_bin_example/GCC/arm_fft_bin_data.c
#if (RUN_FROM_RAM)
__attribute__ ((section(".fast")))  // Place the following function in RAM
#endif
void fft_test() 
{
    const uint32_t fftSize = 1024;
    const uint32_t ifftFlag = 0;
    const uint32_t doBitReverse = 1;

    arm_cfft_f32(&arm_cfft_sR_f32_len1024, testInput_f32_10khz, ifftFlag, doBitReverse);
}

void toggle_leds()
{
    for (int i = 0; i < LEDS_NUMBER; i++)
    {
        bsp_board_led_invert(i);
        nrf_delay_ms(500);
    }
}

void toggle_test_led()
{
    bsp_board_led_invert(0);
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
    #if (RUN_WITH_CACHE)
    cache_enable();
    #endif

    const int test_samples = 1000;
    bsp_board_init(BSP_INIT_LEDS);


    while (true)
    {
        toggle_test_led();
        for (int i = 0; i < test_samples; i++) {
            fft_test();
        }
    }
}

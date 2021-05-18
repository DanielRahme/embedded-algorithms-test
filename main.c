
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "cache.h"

// FFT stuff
#include "arm_math.h"
#include "arm_const_structs.h"

#define RUN_FROM_RAM 1

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

void blink_start_seq()
{
    const int delay = 250;
    for (int i = 0; i < 3; i++) {
        bsp_board_led_on(0);
        nrf_delay_ms(delay);
        bsp_board_led_off(0);
        nrf_delay_ms(delay);
    }
}


int main(void)
{
#if (RUN_FROM_RAM)
    // Section image located in flash 
    extern const unsigned char __my_cmsis_dsp_load_start__[];
    extern const unsigned char __my_cmsis_dsp_load_end__[];

    // Where to locate the section image in RAM.
    extern unsigned char __my_cmsis_dsp_start__[];
    extern unsigned char __my_cmsis_dsp_end__[];

    /* Copy image from flash to RAM. */
    memcpy(__my_cmsis_dsp_start__,
          __my_cmsis_dsp_load_start__,
          __my_cmsis_dsp_end__ - __my_cmsis_dsp_start__);
#endif

    const int test_samples = 1000;
    bsp_board_init(BSP_INIT_LEDS);
    const int delay = 250;

    // Start sequence
    for (int i = 0; i < 5; i++) {
        bsp_board_led_invert(0);
        nrf_delay_ms(delay);
    }

    // Run FFT 1000 times
    bsp_board_led_invert(0);
    for (int i = 0; i < test_samples; i++) {
        fft_test();
    }


    // Start sequence
    for (int i = 0; i < 5; i++) {
        bsp_board_led_invert(0);
        nrf_delay_ms(delay);
    }

    // Enable cache
    cache_enable();

    // Run FFT with cache enabled
    bsp_board_led_invert(0);
    for (int i = 0; i < test_samples; i++) {
        fft_test();
    }

    // End
    bsp_board_led_invert(0);

    while (true)
    {
    }
}

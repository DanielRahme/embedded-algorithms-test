
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "cache.h"


// FFT stuff
#include "arm_math.h"
#include "arm_const_structs.h"
#define TEST_LENGTH_SAMPLES 2048

extern float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES];
static float32_t testOutput[TEST_LENGTH_SAMPLES/2];

// Example from: 
// https://github.com/ARM-software/CMSIS/blob/master/CMSIS/DSP_Lib/Examples/arm_fft_bin_example/GCC/arm_fft_bin_data.c
int fft_test() 
{
    uint32_t fftSize = 1024;
    uint32_t ifftFlag = 0;
    uint32_t doBitReverse = 1;

    /* Reference index at which max energy of bin ocuurs */
    uint32_t refIndex = 213, testIndex = 0;

    arm_status status;
    float32_t maxValue;

    status = ARM_MATH_SUCCESS;

    /* Process the data through the CFFT/CIFFT module */
    arm_cfft_f32(&arm_cfft_sR_f32_len1024, testInput_f32_10khz, ifftFlag, doBitReverse);

    /* Process the data through the Complex Magnitude Module for
    calculating the magnitude at each bin */
    arm_cmplx_mag_f32(testInput_f32_10khz, testOutput, fftSize);

    /* Calculates maxValue and returns corresponding BIN value */
    arm_max_f32(testOutput, fftSize, &maxValue, &testIndex);

    if(testIndex !=  refIndex) {
        status = ARM_MATH_TEST_FAILURE;
    }

    if( status != ARM_MATH_SUCCESS) {
        while(1);   // Fail
    }
    return 0;
}

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

    fft_test();

    while (true)
    {
        blink(blink_delay);
    }
}

/**
 ******************************************************************************
 * @file    main.c
 * @author  Weili An, Niraj Menon
 * @date    Jan 31 2024
 * @brief   ECE 362 Lab 5 Student template
 ******************************************************************************
 */

/*******************************************************************************/

// Fill out your username, otherwise your completion code will have the
// wrong username!
const char *username = "kolb16";

/*******************************************************************************/

#include "stm32f0xx.h"
#include <math.h> // for M_PI

void nano_wait(int);

// 16-bits per digit.
// The most significant 8 bits are the digit number.
// The least significant 8 bits are the segments to illuminate.
uint16_t msg[8] = {0x0000, 0x0100, 0x0200, 0x0300, 0x0400, 0x0500, 0x0600, 0x0700};
extern const char font[];
// Print an 8-character string on the 8 digits
void print(const char str[]);
// Print a floating-point value.
void printfloat(float f);

void autotest(void);

//============================================================================
// PWM Lab Functions
//============================================================================
void setup_tim3(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIOC->MODER &= ~0xFF000;
    GPIOC->MODER |= 0xAA000;
    GPIOC->AFR[1] &= ~0xFF;
    TIM3->PSC = 47999;
    TIM3->ARR = 999;
    TIM3->CCMR1 |= 0x6060;
    TIM3->CCMR2 |= 0x6060;
    TIM3->CCER |= 0x1111;
    TIM3->CCR1 = 800;
    TIM3->CCR2 = 600;
    TIM3->CCR3 = 400;
    TIM3->CCR4 = 200;

    TIM3->CR1 |= TIM_CR1_CEN;
}

void setup_tim1(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER &= ~0xFF0000;
    GPIOA->MODER |= 0xAA0000;
    GPIOA->AFR[1] |= 0x2222;
    TIM1->BDTR |= 0x8000;
    TIM1->PSC = 0;
    TIM1->ARR = 2399;
    TIM1->CCMR1 |= 0x6860;
    TIM1->CCMR2 |= 0x6860;
    TIM1->CCER |= 0x1111;
    TIM1->CR1 |= TIM_CR1_CEN;
}

int getrgb(void);

// Helper function for you
// Accept a byte in BCD format and convert it to decimal
uint8_t bcd2dec(uint8_t bcd)
{
    // Lower digit
    uint8_t dec = bcd & 0xF;

    // Higher digit
    dec += 10 * (bcd >> 4);
    return dec;
}

void setrgb(int rgb)
{
    uint8_t b = bcd2dec(rgb & 0xFF);
    uint8_t g = bcd2dec((rgb >> 8) & 0xFF);
    uint8_t r = bcd2dec((rgb >> 16) & 0xFF);

    // TODO: Assign values to TIM1->CCRx registers
    // Remember these are all percentages
    // Also, LEDs are on when the corresponding PWM output is low
    // so you might want to invert the numbers.
    TIM1->CCR1 = 2400 - r * 24;
    TIM1->CCR2 = 2400 - g * 24;
    TIM1->CCR3 = 2400 - b * 24;
}

//============================================================================
// Lab 4 code
// Add in your functions from previous lab
//============================================================================

// Part 3: Analog-to-digital conversion for a volume level.
uint32_t volume = 2400;

// Variables for boxcar averaging.
#define BCSIZE 32
int bcsum = 0;
int boxcar[BCSIZE];
int bcn = 0;

void dialer(void);

// Parameters for the wavetable size and expected synthesis rate.
#define N 1000
#define RATE 20000
short int wavetable[N];
int step0 = 0;
int offset0 = 0;
int step1 = 0;
int offset1 = 0;

//============================================================================
// enable_ports()
//============================================================================
void enable_ports(void)
{
    // Enable the clock to GPIOB and GPIOC
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

    GPIOB->MODER &= ~0xFFFFF;
    GPIOB->MODER |= 0x155555;

    GPIOC->MODER &= ~0xFF00;
    GPIOC->MODER |= 0x5500;

    GPIOC->OTYPER |= 0xF0;

    GPIOC->MODER &= ~0xFF;
    GPIOC->PUPDR &= ~0xFF;
    GPIOC->PUPDR |= 0x55;
}

//============================================================================
// setup_dma() + enable_dma()
//============================================================================
void setup_dma(void)
{
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    DMA1_Channel5->CCR &= ~DMA_CCR_EN;
    DMA1_Channel5->CMAR = msg;
    DMA1_Channel5->CPAR = &GPIOB->ODR;
    DMA1_Channel5->CNDTR = 8;

    DMA1_Channel5->CCR = 0x000005B0;
}

void enable_dma(void)
{
    DMA1_Channel5->CCR |= DMA_CCR_EN;
}

//============================================================================
// init_tim15()
//============================================================================
void init_tim15(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;

    TIM15->PSC = 47;
    TIM15->ARR = 999;

    TIM15->DIER |= TIM_DIER_UDE;
    TIM15->CR1 |= TIM_CR1_CEN;
}

//=============================================================================
// Part 2: Debounced keypad scanning.
//=============================================================================

uint8_t col; // the column being scanned

void drive_column(int);                 // energize one of the column outputs
int read_rows();                        // read the four row inputs
void update_history(int col, int rows); // record the buttons of the driven column
char get_key_event(void);               // wait for a button event (press or release)
char get_keypress(void);                // wait for only a button press event.
float getfloat(void);                   // read a floating-point number from keypad
void show_keys(void);                   // demonstrate get_key_event()

//============================================================================
// The Timer 7 ISR
//============================================================================
void TIM7_IRQHandler(void)
{
    TIM7->SR &= ~TIM_SR_UIF;

    int rows = read_rows();
    update_history(col, rows);
    col = (col + 1) & 3;
    drive_column(col);
}

//============================================================================
// init_tim7()
//============================================================================
void init_tim7(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;

    TIM7->PSC = 47;
    TIM7->ARR = 999;

    TIM7->DIER |= TIM_DIER_UIE;
    TIM7->CR1 |= TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM7_IRQn);
}

//============================================================================
// setup_adc()
//============================================================================
void setup_adc(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= 0xC;

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    RCC->CR2 |= RCC_CR2_HSI14ON;
    while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0)
    {
    };

    ADC1->CR |= ADC_CR_ADEN;
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0)
    {
    };

    ADC1->CHSELR = ADC_CHSELR_CHSEL1;
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0)
    {
    };
}

//============================================================================
// Timer 2 ISR
//============================================================================
// Write the Timer 2 ISR here.  Be sure to give it the right name.
void TIM2_IRQHandler(void)
{
    TIM2->SR &= ~TIM_SR_UIF;
    ADC1->CR |= ADC_CR_ADSTART;

    while ((ADC1->ISR & ADC_ISR_EOC) == 0)
    {
    };

    bcsum -= boxcar[bcn];
    bcsum += boxcar[bcn] = ADC1->DR;
    bcn += 1;
    if (bcn >= BCSIZE)
        bcn = 0;
    volume = bcsum / BCSIZE;
}

//============================================================================
// init_tim2()
//============================================================================
void init_tim2(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = 479;
    TIM2->ARR = 9999;

    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 3);
}

//============================================================================
// setup_dac()
//============================================================================
void setup_dac(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA->MODER |= 0x300;

    RCC->APB1ENR |= RCC_APB1ENR_DACEN;

    DAC->CR |= 0b000 << 3;
    DAC->CR |= DAC_CR_TEN1;
    DAC->CR |= DAC_CR_EN1;
}

//============================================================================
// Timer 6 ISR
//============================================================================
// Write the Timer 6 ISR here.  Be sure to give it the right name.
void TIM6_DAC_IRQHandler(void)
{
    //     increment offset0 by step0
    // increment offset1 by step1
    // if offset0 is >= (N << 16)
    //     decrement offset0 by (N << 16)
    // if offset1 is >= (N << 16)
    //     decrement offset1 by (N << 16)

    // int samp = sum of wavetable[offset0>>16] and wavetable[offset1>>16]
    // multiply samp by volume
    // shift samp right by 17 bits to ensure it's in the right format for `DAC_DHR12R1`
    // increment samp by 2048
    // copy samp to DAC->DHR12R1

    TIM6->SR &= ~TIM_SR_UIF;

    offset0 += step0;
    if (offset0 >= (N << 16))
    {
        offset0 -= (N << 16);
    }
    offset1 += step1;
    if (offset1 >= (N << 16))
    {
        offset1 -= (N << 16);
    }
    int samp = (wavetable[offset0 >> 16] + wavetable[offset1 >> 16]);
    samp *= volume;
    samp = samp >> 18;
    samp += 1200;
    TIM1->CCR4 = samp;
}

//============================================================================
// init_tim6()
//============================================================================
void init_tim6(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    TIM6->PSC = 47;
    TIM6->ARR = ((48000000 / RATE) / 48) - 1;
    TIM6->DIER |= TIM_DIER_UIE;
    TIM6->CR1 |= TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

//===========================================================================
// init_wavetable()
// Write the pattern for a complete cycle of a sine wave into the
// wavetable[] array.
//===========================================================================
void init_wavetable(void)
{
    for (int i = 0; i < N; i++)
        wavetable[i] = 32767 * sin(2 * M_PI * i / N);
}

//============================================================================
// set_freq()
//============================================================================
void set_freq(int chan, float f)
{
    if (chan == 0)
    {
        if (f == 0.0)
        {
            step0 = 0;
            offset0 = 0;
        }
        else
            step0 = (f * N / RATE) * (1 << 16);
    }
    if (chan == 1)
    {
        if (f == 0.0)
        {
            step1 = 0;
            offset1 = 0;
        }
        else
            step1 = (f * N / RATE) * (1 << 16);
    }
}

//============================================================================
// All the things you need to test your subroutines.
//============================================================================
int main(void)
{
    internal_clock();

    // Uncomment autotest to get the confirmation code.
    autotest();

    // Demonstrate part 1
// #define TEST_TIMER3
#ifdef TEST_TIMER3
    setup_tim3();
    for (;;)
    {
    }
#endif

    // Initialize the display to something interesting to get started.
    msg[0] |= font['E'];
    msg[1] |= font['C'];
    msg[2] |= font['E'];
    msg[3] |= font[' '];
    msg[4] |= font['3'];
    msg[5] |= font['6'];
    msg[6] |= font['2'];
    msg[7] |= font[' '];

    enable_ports();
    setup_dma();
    enable_dma();
    init_tim15();
    init_tim7();
    setup_adc();
    init_tim2();
    init_wavetable();
    init_tim6();

    setup_tim1();

    // demonstrate part 2
// #define TEST_TIM1
#ifdef TEST_TIM1
    for (;;)
    {
        // Breathe in...
        for (float x = 1; x < 2400; x *= 1.1)
        {
            TIM1->CCR1 = TIM1->CCR2 = TIM1->CCR3 = 2400 - x;
            nano_wait(100000000);
        }
        // ...and out...
        for (float x = 2400; x >= 1; x /= 1.1)
        {
            TIM1->CCR1 = TIM1->CCR2 = TIM1->CCR3 = 2400 - x;
            nano_wait(100000000);
        }
        // ...and start over.
    }
#endif

    // demonstrate part 3
// #define MIX_TONES
#ifdef MIX_TONES
    set_freq(0, 1000);
    for (;;)
    {
        char key = get_keypress();
        if (key == 'A')
            set_freq(0, getfloat());
        if (key == 'B')
            set_freq(1, getfloat());
    }
#endif

    // demonstrate part 4
// #define TEST_SETRGB
#ifdef TEST_SETRGB
    for (;;)
    {
        char key = get_keypress();
        if (key == 'A')
            set_freq(0, getfloat());
        if (key == 'B')
            set_freq(1, getfloat());
        if (key == 'D')
            setrgb(getrgb());
    }
#endif

    // Have fun.
    dialer();
}

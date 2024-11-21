#include "stm32f0xx.h"
#include <math.h> // for M_PI
#include <stdio.h>
#include <stdint.h>

void nano_wait(int);
void internal_clock();
<<<<<<< HEAD
void show_char(int col, char n);

uint16_t msg[8] = { 0x0000,0x0100,0x0200,0x0300,0x0400,0x0500,0x0600,0x0700 };
extern const char font[];

uint16_t mode[34] = {
        0x002, // Command to set the cursor at the first position line 1
        0x200+'C', 0x200+'u', 0x200+'r', 0x200+'r', 0x200+'e', 0x200+'n', 0x200+'t', 0x200+' ',
        0x200+'W', 0x200+'i', 0x200+'n', 0x200+'d',  0x200+'i',  0x200+'n', 0x200+'g', 0x200+' ',
        0x0c0, // Command to set the cursor at the first position line 2
        0x200+'S', 0x200+'p', 0x200+'e', 0x200+'e', 0x200+'d', + 0x200+':', 0x200+' ', 0x200 + '1',
        0x200+'0', 0x200+'0', 0x200+' ', 0x200+'R', + 0x200+'P', 0x200+'M', 0x200+' ', 0x200+' ',
};

=======

uint16_t msg[8] = {0x0000, 0x0100, 0x0200, 0x0300, 0x0400, 0x0500, 0x0600, 0x0700};
extern const char font[];

float roto_so_far;
int rpm_val;

uint16_t mode[34] = {
    0x002, // Command to set the cursor at the first position line 1
    0x200 + 'C',
    0x200 + 'u',
    0x200 + 'r',
    0x200 + 'r',
    0x200 + 'e',
    0x200 + 'n',
    0x200 + 't',
    0x200 + ' ',
    0x200 + 'W',
    0x200 + 'i',
    0x200 + 'n',
    0x200 + 'd',
    0x200 + 'i',
    0x200 + 'n',
    0x200 + 'g',
    0x200 + ' ',
    0x0c0, // Command to set the cursor at the first position line 2
    0x200 + 'S',
    0x200 + 'p',
    0x200 + 'e',
    0x200 + 'e',
    0x200 + 'd',
    +0x200 + ':',
    0x200 + ' ',
    0x200 + '1',
    0x200 + '0',
    0x200 + '0',
    0x200 + ' ',
    0x200 + 'R',
    +0x200 + 'P',
    0x200 + 'M',
    0x200 + ' ',
    0x200 + ' ',
};

>>>>>>> Christian
void init_spi1()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= 0x80008800;
    GPIOA->AFR[0] &= ~(0xF0F00000);
    GPIOA->AFR[1] &= ~(0xF0000000);
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI1->CR1 &= ~(0x40);
    SPI1->CR1 |= 0x0038;
    SPI1->CR2 |= 0x0900;
    SPI1->CR2 &= ~(0x0600);
    SPI1->CR2 |= 0x000E;
    SPI1->CR1 |= 0x0044;
}

void spi_cmd(unsigned int data)
{
<<<<<<< HEAD
    while (!(SPI1->SR & 0x2)){
=======
    while (!(SPI1->SR & 0x2))
    {
>>>>>>> Christian
    }
    SPI1->DR = data;
}

void spi1_init_oled()
{
    nano_wait(1000000);
    spi_cmd(0x38);
    spi_cmd(0x08);
    spi_cmd(0x01);
    nano_wait(2000000);
    spi_cmd(0x06);
    spi_cmd(0x02);
    spi_cmd(0x0C);
<<<<<<< HEAD
}

void spi1_setup_dma(void)
{
    DMA1_Channel3->CCR &= ~(0x1);
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1_Channel3->CMAR = mode;
    DMA1_Channel3->CPAR = (&(SPI1->DR));
    DMA1_Channel3->CNDTR = 34;
    DMA1_Channel3->CCR |= (0x5B0);
    SPI1->CR2 |= 0x80;
}

void spi1_enable_dma(void)
{
    DMA1_Channel3->CCR |= 0x1;
}

int adc_to_rpm(int adc){
    //adc 0->4095
    return (int)(((float)adc)/40.95);
}

void setup_adc(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= 0xC;
    ADC1->CFGR2 &= ~(0xC0000000);
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->CR2 |= RCC_CR2_HSI14ON;
    while(RCC->CR2 & 0x2 == 0)
    {
        nano_wait(1);
    }
    ADC1->CR |= ADC_CR_ADEN;
    ADC1->CHSELR = 0;
    while((ADC1->ISR & ADC_ISR_ADRDY) == 0)
    {
        nano_wait(1);
    }
    ADC1->CHSELR |= ADC_CHSELR_CHSEL1; //PA1
    while((ADC1->ISR & ADC_ISR_ADRDY) == 0)
    {
        nano_wait(1);
    }
}

//============================================================================
// Varables for boxcar averaging.
//============================================================================
#define BCSIZE 32
int bcsum = 0;
int boxcar[BCSIZE];
int bcn = 0;
uint32_t adc_val = 2048;
=======
}

void spi1_setup_dma(void)
{
    DMA1_Channel3->CCR &= ~(0x1);
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1_Channel3->CMAR = mode;
    DMA1_Channel3->CPAR = (&(SPI1->DR));
    DMA1_Channel3->CNDTR = 34;
    DMA1_Channel3->CCR |= (0x5B0);
    SPI1->CR2 |= 0x80;
}

void spi1_enable_dma(void)
{
    DMA1_Channel3->CCR |= 0x1;
}

int adc_to_rpm(int adc)
{
    // adc 0->4095
    return (((adc * 1.2) - 2048) / 250) + 5;
}

int adc_to_rpm_norm(int adc)
{
    // adc 0->4095
    return (int)(adc / 4);
}

void setup_adc(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= 0xC;
    ADC1->CFGR2 &= ~(0xC0000000);
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->CR2 |= RCC_CR2_HSI14ON;
    while (RCC->CR2 & 0x2 == 0)
    {
        nano_wait(1);
    }
    ADC1->CR |= ADC_CR_ADEN;
    ADC1->CHSELR = 0;
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0)
    {
        nano_wait(1);
    }
    ADC1->CHSELR |= ADC_CHSELR_CHSEL1; // PA1
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0)
    {
        nano_wait(1);
    }
}
>>>>>>> Christian

//============================================================================
// Varables for boxcar averaging.
//============================================================================
#define BCSIZE 32
int bcsum = 0;
int boxcar[BCSIZE];
int bcn = 0;
uint32_t adc_val = 2048;

//============================================================================
// Timer 2 ISR
//============================================================================
// Write the Timer 2 ISR here.  Be sure to give it the right name.
void TIM2_IRQHandler(void){
    TIM2->SR &= ~TIM_SR_UIF;
    ADC1->CR |= ADC_CR_ADSTART;
<<<<<<< HEAD
    while(ADC_ISR_EOC == 0)
=======
    while (ADC_ISR_EOC == 0)
>>>>>>> Christian
    {
        nano_wait(1);
    }
    bcsum -= boxcar[bcn];
    bcsum += boxcar[bcn] = ADC1->DR;
    bcn += 1;
    if (bcn >= BCSIZE)
        bcn = 0;
    adc_val = bcsum / BCSIZE;
}


//============================================================================
// init_tim2()
//============================================================================
void init_tim2(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->ARR = (48000) - 1;
    TIM2->PSC = 99;
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] = 1 << TIM2_IRQn;
    TIM2->CR1 |= TIM_CR1_CEN;
<<<<<<< HEAD
}

void enable_7SEG_ports(){
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~(0x003FFFFF);
    GPIOB->MODER |= 0x00155555;
}

void setup_dma(void) {
    DMA1_Channel5->CCR &= ~(0x1);
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1_Channel5->CMAR = msg;
    DMA1_Channel5->CPAR = &(GPIOB->ODR);
    DMA1_Channel5->CNDTR = 8;
    //DMA1->CCR &= ~(0x50);
    DMA1_Channel5->CCR |= 0x000005B0;
}

void enable_dma(void) {
    DMA1_Channel5->CCR |= (0x1);
}

//============================================================================
// init_tim15()
//============================================================================
void init_tim15(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
    TIM15->DIER |= 0x100;
    TIM15->PSC = 0;
    TIM15->ARR = (48000-1);
    //NVIC->ISER |= TIM15_IRQn;
    //en
    TIM15->CR1 |= 0x1;
}

int main(void) {
    internal_clock();
    int hundreds = 0;
    int tens = 0;
    int ones = 0;
    int rpm_val;
    int hr, tens_min, ones_min;
    int time_left;
    hr = 9;
    ones_min = 9;
    tens_min = 9;
    float roto_so_far = 0;
    msg[0] |= font['T'];
    msg[1] |= font['L'];
    msg[2] |= font[' '];
    msg[3] |= font['0'];
    msg[4] |= font['h'];
    msg[5] |= font['r'];
    msg[6] |= font['0'];
    msg[7] |= font['0'];
    //ascii ' ' is 32
    //ascii 0 is 48
=======
}

void enable_7SEG_ports()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~(0x003FFFFF);
    GPIOB->MODER |= 0x00155555;
}

void setup_dma(void)
{
    DMA1_Channel5->CCR &= ~(0x1);
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1_Channel5->CCR = 0;
    DMA1_Channel5->CMAR = msg;
    DMA1_Channel5->CPAR = &(GPIOB->ODR);
    DMA1_Channel5->CNDTR = 8;
    // DMA1->CCR &= ~(0x50);
    // DMA1_Channel5->CCR |= 0x000005B0;
    DMA1_Channel5->CCR |= DMA_CCR_CIRC      // Circular mode
                          | DMA_CCR_DIR     // Read from memory
                          | DMA_CCR_MINC    // Memory increment mode
                          | DMA_CCR_MSIZE_0 // MSIZE = 01 (16 bits)
                          | DMA_CCR_PSIZE_0 // PSIZE = 01 (16 bits)
                          | DMA_CCR_PL_1;
    DMA1_Channel5->CCR |= DMA_CCR_EN;
}

void enable_dma(void)
{
    DMA1_Channel5->CCR |= (0x1);
}

void setup_tim1(void)
{   
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER8_1;
    GPIOA->AFR[1] |= 0x2 << 0;
    TIM1->BDTR |= TIM_BDTR_MOE;
    TIM1->PSC = 0;
    TIM1->ARR = 999;
    TIM1->CCMR1 |= 0b110 << 4;
    TIM1->CCER |= 1 << 0;
    TIM1->CR1 |= TIM_CR1_CEN;  
}

//============================================================================
// init_tim15()
//============================================================================
void init_tim15(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
    TIM15->DIER |= 0x100;
    TIM15->PSC = 0;
    TIM15->ARR = (48000 - 1);
    // NVIC->ISER |= TIM15_IRQn;
    // en
    TIM15->CR1 |= 0x1;
}

int main(void)
{
    internal_clock();
    float roto_so_far = 0;
    int hundreds = 0;
    int tens = 0;
    int ones = 0;
    int rpm_norm;
    int hr, tens_min, ones_min;
    int time_left;
    msg[0] |= font['T'];
    msg[1] |= font['L'];
    msg[2] |= font[' '];
    msg[3] |= font[' '];
    msg[4] |= font[' '];
    msg[5] |= font[' '];
    msg[6] |= font[' '];
    msg[7] |= font[' '];
    // ascii ' ' is 32
    // ascii 0 is 48
>>>>>>> Christian
    enable_7SEG_ports();
    init_spi1();
    spi1_init_oled();
    spi1_setup_dma();
    spi1_enable_dma();
    setup_adc();
    init_tim2();
    setup_dma();
    enable_dma();
    init_tim15();
<<<<<<< HEAD
    mode[25] = 0x200+ (char)(hundreds+48);//hundreds either 1 or 0
    mode[26] = 0x200+ (char)(tens+48);
    mode[27] = 0x200+ (char)(ones+48);
    while(1){
        nano_wait(10000000000); //this is one sec
        rpm_val = adc_to_rpm(adc_val);
        
        // roto_so_far += (rpm_val / 60); //adds the rotations turned over last second
        // time_left = (800.0 - roto_so_far) / ((float)(rpm_val+10));
        
        // hr = time_left / 60;
        // tens_min = (time_left % 60) / 10;
        // ones_min = (time_left % 10);

        hr -= 1;
        tens_min -= 1;
        ones_min -= 1;
        if(hr == -1)
        {
            hr = 10;
            tens_min = 10;
            ones_min = 10;
        }
        hundreds = rpm_val / 100;
        tens = rpm_val / 10;
        ones = rpm_val % 10;
        //oled spi below
        mode[25] = 0x200+ (char)(hundreds+48);//hundreds either 1 or 0
        mode[26] = 0x200+ (char)(tens+48);
        mode[27] = 0x200+ (char)(ones+48);
        //7 seg below
        // total_roto / rpm_val = 
        msg[0] |= font['T'];
        msg[1] |= font['L'];
        msg[2] |= font[' '];
        msg[3] |= font[(char)(hr + 48)];
        msg[4] |= font['h'];
        msg[5] |= font['r'];
        msg[6] |= font[(char)(tens_min+48)];
        msg[7] |= font[(char)(ones_min+48)];
    }
}
=======
    setup_tim1();
    mode[25] = 0x200 + (char)(hundreds + 48); // hundreds either 1 or 0
    mode[26] = 0x200 + (char)(tens + 48);
    mode[27] = 0x200 + (char)(ones + 48);
    while (1)
    {
        nano_wait(1000000000); // this is one sec
        rpm_val = adc_to_rpm(adc_val);
        
        rpm_norm = adc_to_rpm_norm(adc_val);        
        if(rpm_norm > 1000){
            rpm_norm = 1000;
        }
        TIM1->CCR1 = (1000 - rpm_norm);
        roto_so_far += (((float)rpm_val) / 60.0);
        time_left = (800.0 - roto_so_far) / (((float)(rpm_val)) + .001);

        hr = time_left / 60;
        tens_min = (time_left % 60) / 10;
        ones_min = (time_left % 10);

        hundreds = rpm_val / 100;
        tens = rpm_val / 10;
        ones = rpm_val % 10;
        // oled spi below
        mode[25] = 0x200 + (char)(hundreds + 48); // hundreds either 1 or 0
        mode[26] = 0x200 + (char)(tens + 48);
        mode[27] = 0x200 + (char)(ones + 48);
        for (int i = 0; i < 8; i++)
        {
            msg[i] &= 0xFF00; // Clear the lower 8 bits
        }
        if(hr <= 0 && tens_min <= 0 && ones_min <= 0)
        {
            msg[0] |= font['D'];
            msg[1] |= font['O'];
            msg[2] |= font['N'];
            msg[3] |= font['E'];
            msg[4] |= font[' '];
            msg[5] |= font[' '];
            msg[6] |= font[' '];
            msg[7] |= font[' '];
            roto_so_far = 800;
        }
        else
        {
            msg[0] |= font['T'];
            msg[1] |= font['L'];
            msg[2] |= font[' '];
            msg[4] |= font['H'];
            msg[5] |= font['r'];
            msg[3] |= font[(char)(hr + 48)];
            msg[6] |= font[(char)(tens_min + 48)];
            msg[7] |= font[(char)(ones_min + 48)];
        }
    }
}
>>>>>>> Christian



#include "TM1637.h"


void _tm1637Start(void);
void _tm1637Stop(void);
void _tm1637ReadResult(void);
void _tm1637WriteByte(unsigned char b);
void _tm1637DelayUsec(unsigned int i);
void _tm1637ClkHigh(void);
void _tm1637ClkLow(void);
void _tm1637DioHigh(void);
void _tm1637DioLow(void);

// Configuration.

// CLK_PORT GPIOC
// DIO_PORT GPIOC
// CLK_PIN GPIO_PIN_0
// DIO_PIN GPIO_PIN_1

#define PORTC_CLK_ENABLE RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN 


#define CLK_PIN_ENABLE GPIOC->MODER|=(1<<0)|(0<<1)
#define DIO_PIN_ENABLE GPIOC->MODER|=(1<<2)|(0<<3)


const char segmentMap[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, // 0-7
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, // 8-9, A-F
    0x00
};


void tm1637Init(void)
{
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;
  GPIOC->MODER|=(1<<0)|(0<<1);
	GPIOC->MODER|=(1<<2)|(0<<3);
	GPIOC->OSPEEDR|=0xF;
	GPIOC->OTYPER|=(1<<0)|(1<<0);
	GPIOC->PUPDR|=(1<<0)|(1<<2);

    tm1637SetBrightness(8);
}

void tm1637DisplayDecimal(int v, int displaySeparator)
{
    unsigned char digitArr[4];
    for (int i = 0; i < 4; ++i) {
        digitArr[i] = segmentMap[v % 10];
        if (i == 2 && displaySeparator) {
            digitArr[i] |= 1 << 7;
        }
        v /= 10;
    }

    _tm1637Start();
    _tm1637WriteByte(0x40);
    _tm1637ReadResult();
    _tm1637Stop();

    _tm1637Start();
    _tm1637WriteByte(0xc0);
    _tm1637ReadResult();

    for (int i = 0; i < 4; ++i) {
        _tm1637WriteByte(digitArr[3 - i]);
        _tm1637ReadResult();
    }

    _tm1637Stop();
}

// Valid brightness values: 0 - 8.
// 0 = display off.
void tm1637SetBrightness(char brightness)
{
    // Brightness command:
    // 1000 0XXX = display off
    // 1000 1BBB = display on, brightness 0-7
    // X = don't care
    // B = brightness
    _tm1637Start();
    _tm1637WriteByte(0x87 + brightness);
    _tm1637ReadResult();
    _tm1637Stop();
}

void _tm1637Start(void)
{
    _tm1637ClkHigh();
    _tm1637DioHigh();
    _tm1637DelayUsec(20);
    _tm1637DioLow();
}

void _tm1637Stop(void)
{
    _tm1637ClkLow();
    _tm1637DelayUsec(20);
    _tm1637DioLow();
    _tm1637DelayUsec(20);
    _tm1637ClkHigh();
    _tm1637DelayUsec(20);
    _tm1637DioHigh();
}

void _tm1637ReadResult(void)
{
    _tm1637ClkLow();
    _tm1637DelayUsec(50);
    // while (dio); // We're cheating here and not actually reading back the response.
    _tm1637ClkHigh();
    _tm1637DelayUsec(20);
    _tm1637ClkLow();
}

void _tm1637WriteByte(unsigned char b)
{
    for (int i = 0; i < 8; ++i) {
        _tm1637ClkLow();
        if (b & 0x01) {
            _tm1637DioHigh();
        }
        else {
            _tm1637DioLow();
        }
        _tm1637DelayUsec(30);
        b >>= 1;
        _tm1637ClkHigh();
        _tm1637DelayUsec(30);
    }
}



void _tm1637ClkHigh(void)
{
    GPIOC->ODR|=(1<<0);
}

void _tm1637ClkLow(void)
{
    GPIOC->ODR&=~(1<<0);
}

void _tm1637DioHigh(void)
{
    GPIOC->ODR|=(1<<1);
}

void _tm1637DioLow(void)
{
    GPIOC->ODR&=~(1<<1);
}

void _tm1637DelayUsec(unsigned int i)
{
delayuS(i);
}


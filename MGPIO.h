#ifndef __MGPIO_H_
#define __MGPIO_H_

#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

// =================================================================================
// GPIO base
// =================================================================================
#define GPIO_NUM_MIN 2
#define GPIO_NUM_MAX 27

#define FUNC_INPUT  0b000
#define FUNC_OUTPUT 0b001

#define PULL_NO     0b00
#define PULL_UP     0b01
#define PULL_DOWN   0b10

// =================================================================================
// Macros
// =================================================================================
#define MASK(n) (0xffffffff >> (32 - (n)))
#define BIT(a, b) (((a) >> (b)) & 1)
#define BITS(dat, from, width) ((dat >> (from)) & MASK(width))

// =================================================================================
// Debug
// =================================================================================
#define DEBUG_PRINT
#ifdef DEBUG_PRINT
#define DP(...) printf(__VA_ARGS__)
#else
#define DP(...)
#endif

// =================================================================================
// Global GPIO pointer (defined in MGPIO.c)
// =================================================================================
extern volatile uint32_t *gpio;

// =================================================================================
// Prototypes
// =================================================================================
int  MGPIO_Init(void);
void func_sel(uint32_t gpio_num, uint32_t fn);
void set_pud(uint32_t gpio_num, uint32_t pud);
uint32_t gpio_read(uint32_t from, uint32_t width);
void gpio_write(uint32_t bit_pos, uint32_t val, uint32_t width);

void finalize(void);

#ifdef __cplusplus
}
#endif

#endif // __MGPIO_H_

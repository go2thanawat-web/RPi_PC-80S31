//
// Minatsu GPIO Library (modernized)
// Ported for Raspberry Pi OS Bullseye / Bookworm
//

#include "MGPIO.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// ======================================================================
// GPIO register map (BCM283x / BCM2711 compatible for gpiomem)
// ======================================================================
#define GPIO_BLOCK_SIZE 4096

#define GPFSEL0   0   // GPIO Function Select 0
#define GPSET0    7   // GPIO Pin Output Set 0
#define GPCLR0    10  // GPIO Pin Output Clear 0
#define GPLEV0    13  // GPIO Pin Level 0

#define GPPUD     37  // GPIO Pull-up/down (legacy)
#define GPPUDCLK0 38  // GPIO Pull-up/down Clock 0

// ======================================================================
// Global GPIO pointer
// ======================================================================
volatile uint32_t *gpio = NULL;

// ======================================================================
// Initialize GPIO (using /dev/gpiomem)
// ======================================================================
int MGPIO_Init(void) {
    int fd = open("/dev/gpiomem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("open /dev/gpiomem failed");
        return -1;
    }

    gpio = (volatile uint32_t *)mmap(
        NULL,
        GPIO_BLOCK_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    );
    close(fd);

    if (gpio == MAP_FAILED) {
        perror("mmap failed");
        gpio = NULL;
        return -1;
    }

    return 0;
}

// ======================================================================
// GPIO function select
// ======================================================================
void func_sel(uint32_t gpio_num, uint32_t fn) {
    assert(gpio != NULL);
    assert(gpio_num >= GPIO_NUM_MIN && gpio_num <= GPIO_NUM_MAX);
    assert(fn <= 0b111);

    uint32_t reg = GPFSEL0 + (gpio_num / 10);
    uint32_t shift = (gpio_num % 10) * 3;

    uint32_t v = gpio[reg];
    v &= ~(0b111 << shift);
    v |= (fn << shift);
    gpio[reg] = v;
}

// ======================================================================
// Pull-up / Pull-down control (legacy method – still works via gpiomem)
// ======================================================================
void set_pud(uint32_t gpio_num, uint32_t pud) {
    assert(gpio != NULL);
    assert(gpio_num >= GPIO_NUM_MIN && gpio_num <= GPIO_NUM_MAX);
    assert(pud <= 0b10);

    gpio[GPPUD] = pud;
    usleep(5);

    gpio[GPPUDCLK0 + (gpio_num / 32)] = (1 << (gpio_num % 32));
    usleep(5);

    gpio[GPPUD] = 0;
    gpio[GPPUDCLK0 + (gpio_num / 32)] = 0;
}

// ======================================================================
// Read GPIO
// ======================================================================
uint32_t gpio_read(uint32_t from, uint32_t width) {
    assert(gpio != NULL);
    assert(from < 32 && (from + width) <= 32);

    uint32_t v = gpio[GPLEV0];
    return (v >> from) & MASK(width);
}

// ======================================================================
// Write GPIO
// ======================================================================
void gpio_write(uint32_t bit_pos, uint32_t val, uint32_t width) {
    assert(gpio != NULL);
    assert(bit_pos < 32 && (bit_pos + width) <= 32);

    uint32_t mask = MASK(width) << bit_pos;

    // Clear first
    gpio[GPCLR0] = mask;
    // Set required bits
    gpio[GPSET0] = (val << bit_pos) & mask;
}

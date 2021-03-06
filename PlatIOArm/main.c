#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>

// #define X_STEP_PIN                          PC0
// #define X_DIR_PIN                           PB2
// #define X_ENABLE_PIN                        PC13

static uint8_t data = 'A';

static void clock_setup(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    /* Enable GPIOA clock. */
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOB);

	/* Enable clocks for GPIO port A (for GPIO_USART1_TX) and USART1. */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_USART1);
}

static void usart_setup(void)
{
    // /* Enable the USART1 interrupt. */
    nvic_enable_irq(NVIC_USART1_IRQ);

    /* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port A for transmit. */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

    /* Setup GPIO pin GPIO_USART1_RE_RX on GPIO port A for receive. */
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

    /* Setup UART parameters. */
    usart_set_baudrate(USART1, 115200);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    usart_set_mode(USART1, USART_MODE_TX_RX);

    /* Enable USART1 Receive interrupt. */
	USART_CR1(USART1) |= USART_CR1_RXNEIE;

    /* Finally enable the USART. */
    usart_enable(USART1);
}

static void gpio_setup(void)
{
    // gpio_set(GPIOA, GPIO8);
    // gpio_set(GPIOC,GPIO8);
    // gpio_clear(GPIOC,GPIO9);

    /* Setup GPIO5 (in GPIO port A) for LED use. */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO8);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO2);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO0 | GPIO8 | GPIO9 | GPIO13);

    // gpio_set(GPIOA, GPIO8);
    // gpio_clear(GPIOA, GPIO8);
    // gpio_toggle(GPIOA, GPIO8);
}

void usart1_isr(void)
{

	/* Check if we were called because of RXNE. */
	if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {

		/* Retrieve the data from the peripheral. */
		data = usart_recv(USART1);

		/* Enable transmit interrupt so it sends back the data. */
		// USART_CR1(USART1) |= USART_CR1_TXEIE;
	}

	// /* Check if we were called because of TXE. */
	// if (((USART_CR1(USART1) & USART_CR1_TXEIE) != 0) &&
	//     ((USART_SR(USART1) & USART_SR_TXE) != 0)) {

	// 	/* Put data into the transmit register. */
	// 	usart_send(USART1, data);

	// 	/* Disable the TXE interrupt as we don't need it anymore. */
	// 	USART_CR1(USART1) &= ~USART_CR1_TXEIE;
	// }
}

int main(void)
{
    // SCB_VTOR = (uint32_t)0x08005000;

    clock_setup();
    gpio_setup();
    usart_setup();

    // gpio_clear(GPIOC, GPIO13);
    gpio_set(GPIOC, GPIO8);

    int ramp = 7200000;
    while(1)
    {
        // usart_send(USART1, data);
        if(data == 't')
        {
            gpio_toggle(GPIOC, GPIO9);
            gpio_toggle(GPIOC, GPIO0);
            // data = 'U';
        }else if(data == 'e'){
             gpio_toggle(GPIOC, GPIO13);
             data = 'A';
        }else if (data == 'd'){
            gpio_toggle(GPIOB, GPIO2);
            data = 'A';
        }
        if(ramp > 8){
            ramp -= 10000;
        }
        for(int i = 0; i < ramp; i++)
        {
            __asm__("nop");
        }
    }    

    return 0;
}

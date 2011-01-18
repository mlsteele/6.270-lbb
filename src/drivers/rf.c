#ifndef SIMULATE

// Include headers from OS
#include <config.h>
#include <hal/delay.h>
#include <rf.h>
#include <string.h> //memcpy
#include <nrf24l01.h>
#include <kern/global.h>
#include <kern/thread.h>
#include <kern/lock.h>
#include <avr/interrupt.h>
#include <rf.h>
#include <hal/spi.h>
#include <hal/io.h>
#include <motor.h>
#include <kern/thread.h>

#else

#include <joyos.h>
#include <stdarg.h>
#include <socket.h>
#include <stdio.h>

#endif

volatile board_coord objects[4];
volatile uint32_t position_microtime;

#ifndef SIMULATE

packet_buffer tx, rx;

volatile board_coord goal_position; //The target position received from a goal packet

FILE rfio = FDEV_SETUP_STREAM(rf_put, rf_get, _FDEV_SETUP_RW);

volatile char rf_str_buf[PAYLOAD_SIZE+1];
volatile uint8_t rf_buf_index = PAYLOAD_SIZE;

uint8_t rf_ch_count = 0;

volatile uint8_t rf_new_str;

struct lock rf_lock;

volatile uint8_t light_port = 0xFF;
volatile uint8_t robot_id = 0xFF;

volatile uint8_t caught[4];

void rf_status_update(uint8_t caught){

    extern volatile uint8_t robot_id;

    ATOMIC_BEGIN;

    tx.type = STATUS;
    tx.address = 0xFF;
    tx.payload.status.id = robot_id;
    tx.payload.status.caught = caught;

    rf_send_packet(0xE7, (uint8_t*)(&tx), sizeof(packet_buffer));

    ATOMIC_END;
}

int rf_send(char ch){
    ATOMIC_BEGIN;

    tx.payload.array[rf_ch_count++] = ch;

    if ((ch=='\n') || (rf_ch_count == PAYLOAD_SIZE)){
        tx.type = STRING;
        rf_send_packet(0xE7, (uint8_t*)(&tx), sizeof(packet_buffer));
        rf_ch_count = 0;
    }

    ATOMIC_END;

    return ch;
}

int rf_put(char ch, FILE *f) {
    if (ch == '\n')
        rf_send('\r');

    return rf_send(ch);
}


int rf_vprintf(const char *fmt, va_list ap) {
    int count;
    acquire(&rf_lock);
    count = vfprintf(&rfio, fmt, ap);
    release(&rf_lock);

    return count;
}

#endif

int rf_printf(const char *fmt, ...) {
    va_list ap;
    int count;

    va_start(ap, fmt);
	#ifndef SIMULATE
    count = rf_vprintf(fmt, ap);
	#else
	count = vprintf(fmt, ap);
	#endif
    va_end(ap);

    return count;
}

#ifndef SIMULATE

int rf_vprintf_P(const char *fmt, va_list ap) {
    int count;
    acquire(&rf_lock);
    count = vfprintf_P(&rfio, fmt, ap);
    release(&rf_lock);

    return count;
}

int rf_printf_P(const char *fmt, ...) {
    va_list ap;
    int count;

    va_start(ap, fmt);
    count = rf_vprintf_P(fmt, ap);
    va_end(ap);

    return count;
}

char rf_recv() {
    while(rf_buf_index==PAYLOAD_SIZE || rf_str_buf[rf_buf_index]=='\0')
        yield();
    return rf_str_buf[rf_buf_index++];
}

int rf_get(FILE * f) {
    return rf_recv();
}

int rf_vscanf(const char *fmt, va_list ap){
    int count;
    acquire(&rf_lock);
    count = vfscanf(&rfio, fmt, ap);
    release(&rf_lock);

    return count;
}

#endif

int rf_scanf(const char *fmt, ...){
    va_list ap;
    int count;

    va_start(ap, fmt);
	#ifndef SIMULATE
    count = rf_vscanf(fmt, ap);
	#else
	count = vscanf(fmt, ap);
	#endif
    va_end(ap);

    return count;
}

#ifndef SIMULATE

int rf_vscanf_P(const char *fmt, va_list ap) {
    int count;
    acquire(&rf_lock);
    count = vfscanf_P(&rfio, fmt,ap);
    release(&rf_lock);

    return count;
}

int rf_scanf_P(const char *fmt, ...) {
    va_list ap;
    int count;

    va_start(ap, fmt);
    count = rf_vscanf_P(fmt, ap);
    va_end(ap);

    return count;
}

uint8_t rf_has_char() {
    return (rf_buf_index != PAYLOAD_SIZE) &&
        (rf_str_buf[rf_buf_index] != '\0');
}

void rf_rx(void) {
    RF_CE(0);
    delay_busy_us(150); // I don't think delay_busy_us actually goes up that high
    nrf_write_reg(NRF_REG_CONFIG,
            _BV(NRF_BIT_PRIM_RX) |
            _BV(NRF_BIT_CRCO) |
            _BV(NRF_BIT_EN_CRC) |
            _BV(NRF_BIT_MASK_MAX_RT) |
            _BV(NRF_BIT_MASK_TX_DR)); // PRX, 16 bit CRC enabled
    nrf_write_reg(NRF_REG_EN_AA, 0); // disable auto-ack for all channels
    nrf_write_reg(NRF_REG_RF_SETUP,
            (NRF_RF_PWR_0DB << NRF_RF_PWR_BASE) |
            (NRF_RF_DR_1MBPS << NRF_BIT_RF_DR_BASE) |
            _BV(NRF_BIT_LNA_HCURR)); // data rate = 1MB
    nrf_write_reg(NRF_REG_RX_PW_P0, sizeof(packet_buffer));
    nrf_write_reg(NRF_REG_CONFIG,
            _BV(NRF_BIT_PRIM_RX) |
            _BV(NRF_BIT_PWR_UP) |
            //_BV(NRF_BIT_CRCO) | // this bit was not in the configuration for some unknown reason
            _BV(NRF_BIT_EN_CRC) |
            _BV(NRF_BIT_MASK_MAX_RT) |
            _BV(NRF_BIT_MASK_TX_DR)); // PWR_UP = 1
    RF_CE(1);
    // wait >= 130 us
    delay_busy_us(150);
}

uint8_t rf_tx(void) {
    RF_CE(0);
    delay_busy_us(150);
    nrf_write_reg(NRF_REG_CONFIG,
            _BV(NRF_BIT_CRCO) |
            _BV(NRF_BIT_EN_CRC) |
            _BV(NRF_BIT_MASK_MAX_RT) |
            _BV(NRF_BIT_MASK_TX_DR) |
            _BV(NRF_BIT_MASK_RX_DR)); //16 bit CRC enabled, be a transmitter
    nrf_write_reg(NRF_REG_EN_AA, 0); //Disable auto acknowledge on all pipes
    nrf_write_reg(NRF_REG_SETUP_RETR, 0); //Disable auto-retransmit
    nrf_write_reg(NRF_REG_SETUP_AW, NRF_AW_5); //Set address width to 5bytes (default, not really needed)
    nrf_write_reg(NRF_REG_RF_SETUP,
            (NRF_RF_PWR_0DB << NRF_RF_PWR_BASE) |
            (NRF_RF_DR_1MBPS << NRF_BIT_RF_DR_BASE) |
            _BV(NRF_BIT_LNA_HCURR)); //Air data rate 1Mbit, 0dBm, Setup LNA
    nrf_write_reg(NRF_REG_RF_CH, 2); //RF Channel 2 (default, not really needed)
    uint8_t addr[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    nrf_write_multibyte_reg(NRF_REG_TX_ADDR, addr, 5);
    nrf_write_reg(NRF_REG_CONFIG,
            _BV(NRF_BIT_PWR_UP) |
            //_BV(NRF_BIT_CRCO) |
            _BV(NRF_BIT_EN_CRC) |
            _BV(NRF_BIT_MASK_MAX_RT) |
            _BV(NRF_BIT_MASK_TX_DR) |
            _BV(NRF_BIT_MASK_RX_DR)); //Power up, be a transmitter
    return nrf_read_status();
}

uint8_t rf_send_packet(uint8_t address, uint8_t *data, uint8_t len) {
    rf_tx();
    // preserve pipe 0 address
    uint8_t pipe0_addr = nrf_read_reg(NRF_REG_RX_ADDR_P0);
    // listen for ACK to this address
    nrf_write_reg(NRF_REG_RX_ADDR_P0, address);
    // write TX address LSB first
    nrf_write_reg(NRF_REG_TX_ADDR, address);

    nrf_write_reg(NRF_REG_STATUS, 0x7E); //Clear any interrupts

    nrf_write_tx_payload(data, len);
    // start transmission
    RF_CE(1);
    delay_busy_us(20);
    RF_CE(0);
    // wait for ACK
    uint8_t status;
    while(((status = nrf_read_reg(NRF_REG_STATUS)) & (_BV(NRF_BIT_TX_DS) | _BV(NRF_BIT_MAX_RT))) == 0) {
        yield();
    }
    //printf("TX ACK status: %02X\n", status);
    // clear transmit interrupt conditions
    nrf_write_reg(NRF_REG_STATUS, status & (_BV(NRF_BIT_TX_DS) | _BV(NRF_BIT_MAX_RT)));
    // restore pipe 0 address
    nrf_write_reg(NRF_REG_RX_ADDR_P0, pipe0_addr);
    // flush TX FIFO
    nrf_flush_tx();
    // return to RX mode
    rf_rx();
    return (status & _BV(NRF_BIT_TX_DS)) != 0;
}

void rf_process_packet (packet_buffer *rx, uint8_t size, uint8_t pipe) {
    uint8_t type = rx->type;
    //uint8_t address = rx->address;

    switch (type) {
        case POSITION:
            if (robot_id != 0xFF && rx->payload.coords[0].id != robot_id) {
                uint8_t i;
                for (i=1; i<4; i++) {
                    if (rx->payload.coords[i].id == robot_id)
                        break;
                }
                if (i != 4) {
                    board_coord t;
                    t = rx->payload.coords[0];
                    rx->payload.coords[0] = rx->payload.coords[i];
                    rx->payload.coords[i] = t;
                }
            }
            memcpy((char *)objects, rx->payload.coords, sizeof(objects));
            position_microtime = get_time_us();
            break;

        case GOAL:
            goal_position = rx->payload.coords[0];
            break;

        case START:
            if (robot_id != 0xFF) {
                //Remaining bytes are robots which are starting.  Check if we're one of them.
                for (uint8_t i = 0; i < 30; i++) {
                    uint8_t target = rx->payload.array[i];
                    if (target == robot_id) { round_start(); break; }
                }
            }
            break;

        case STOP:
            if (robot_id != 0xFF) {
                //Remaining bytes are robots which are stopping.  Check if we're one of them.
                for (uint8_t i = 0; i < 30; i++) {
                    uint8_t target = rx->payload.array[i];
                    if (target == robot_id) { round_end(); break; }
                }
            }
            break;

        case STRING:
            rf_buf_index = 0;
            memcpy((char *)rf_str_buf, rx->payload.array, PAYLOAD_SIZE);
            break;

        case LIGHT:
            if (light_port != 0xFF && robot_id != 0xFF) {
                for (int i=0; i<4; i++) {
                    if (rx->payload.lights[i].id == robot_id)
                        motor_set_vel(light_port, rx->payload.lights[i].value);
                }
            }
            break;

        case STATUS:
            for (uint8_t i=0; i<4; i++){
                if (rx->payload.status.id == objects[i].id){
                    caught[i] = rx->payload.status.caught;
                    break;
                }
            }
            break;
        default:
            break;
    }
}

// get a packet; return pipe number
uint8_t rf_get_packet(uint8_t *buf, uint8_t *size) {
    uint8_t pipe;
    while (1) {
        pipe = ((nrf_read_reg(NRF_REG_STATUS) & NRF_RX_P_NO_MASK) >> NRF_RX_P_NO_BASE);
        if (pipe == NRF_RX_P_NO_EMPTY)
            return pipe;
        *size = nrf_read_rx_payload_len();
        if (*size > 32) {
            nrf_flush_rx();
            continue;
        }
        break;
    }
    nrf_read_rx_payload(buf, *size);
    return pipe;
}

#endif

int rf_receive (void) {

	#ifndef SIMULATE

    for (;;) {
        //if (PINE & _BV(PD7)) {
        uint8_t status = nrf_read_status();
        if (status & _BV(NRF_BIT_RX_DR)) {
            nrf_write_reg(NRF_REG_STATUS, _BV(NRF_BIT_RX_DR)); //reset int

            packet_buffer rx;
            uint8_t size;
            uint8_t pipe;
            while ((pipe = rf_get_packet((uint8_t*)&rx, &size)) != NRF_RX_P_NO_EMPTY)
                rf_process_packet(&rx, size, pipe);
        }
        //}
        yield();
    }
    return 0;

	#else

	float x;
	float y;

	while(1){

		write(sockfd, "x\n", 2);
		read(sockfd, socket_buffer, SOCKET_BUF_SIZE);
		sscanf(socket_buffer, "%f", &x);

		write(sockfd, "y\n", 2);
		read(sockfd, socket_buffer, SOCKET_BUF_SIZE);
		sscanf(socket_buffer, "%f", &y);

		position_microtime = get_time_us();

		pause(10);

	}
  

	#endif

}

// Initialize RF
void rf_init (void) {

	#ifndef SIMULATE

    extern struct thread *current_thread;
    if (current_thread != NULL)
        return;

    // The rf_lock ensures that printing to
    // rf from several threads will not cause
    // characters to be interleaved between threads
    init_lock(&rf_lock, "RF Lock");

    // STRING packets don't contain the null character
    // for efficiency.  rf_str_buf is one character larger
    // than the payload to hold this additional character
    rf_str_buf[PAYLOAD_SIZE] = '\0';

    rf_new_str = 0;

    rf_rx(); //Enable receive mode

	#endif

    create_thread (&rf_receive, STACK_DEFAULT, 0, "rf");
}

#include <esp/gpio.h>
#include <espressif/esp_common.h>
#include <stdio.h>
#include <esp/uart.h>
#include <FreeRTOS.h>
#include <task.h>

#include "roomba/roomba.h"

// PACKETS
static const int PACKET_LEN = 22;

const uint8_t UART_NO = 1;

typedef unsigned char byte;

void update();
void get_state(roomba_state_t *);
void create_packet(byte *packet, roomba_settings_t);
void write_packet(byte *packet, int len);
void get_control(int8_t dd_pin);

bool roomba_init(int8_t dd_pin) {
    return true;
    uart_set_baud(UART_NO, 125200);
    gpio_enable(dd_pin, GPIO_OUTPUT);
    gpio_set_pullup(dd_pin, 1, 1);

    get_control(dd_pin);
    return true;
}


void roomba_update_settings(roomba_settings_t *settings) {
    update(*settings);
}

void roomba_read_state(roomba_state_t *state) {
    get_state(state);
}

void listen_for_state_change(state_update_callback_fn callback) {
    // TODO
}

// Updates actual device with desired_settings
void update(roomba_settings_t settings) {
    printf("Updating: %i", settings.power);
    gpio_enable(2, GPIO_OUTPUT);
    switch (settings.power) {
        case ON:
            gpio_write(2, 0);
            break;
        case OFF:
            gpio_write(2, 1);
            break;
    }

    byte packet[PACKET_LEN];
    create_packet(packet, settings);
    write_packet(packet, PACKET_LEN);
}

void get_state(roomba_state_t *state) {
    state->charging_state = CHARGING;
    state->capacity = 1000;
    state->charge = 600;
}

void create_packet(byte *packet, roomba_settings_t settings) {
    switch (settings.power) {
        case ON:
            packet[0] = 133;
            break;
        case OFF:
            packet[0] = 143;
            break;
    }
}

void write_packet(byte *packet, int len) {
    printf("len: %i\n", len);
    printf("putting: ");
    //memcpy(content, packet.contents, packet.len);
    for (int i = 0; i < len; i++) {
        printf("%x ", packet[i]);
        uart_putc_nowait(UART_NO, packet[i]);
    }
    printf("\n");
}

void get_control(int8_t dd_pin) {
    // wake up the robot
    gpio_write(dd_pin, 1); // Make sure the pin line is high to start
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_write(dd_pin, 0); // 500ms LOW signal wakes up the robot
    vTaskDelay(500 / portTICK_PERIOD_MS);
    gpio_write(dd_pin, 1); // Send it back HIGH once the robot is awake

    vTaskDelay(2000 / portTICK_PERIOD_MS);

    // We need to first set the robot into passive mode, and then safe mode
    uart_putc_nowait(UART_NO, 128); // Passive mode
    vTaskDelay(50 / portTICK_PERIOD_MS);
}

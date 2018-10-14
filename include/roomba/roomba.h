#ifndef __roomba_H__
#define __roomba_H__
#include <stdint.h>

typedef struct {
    enum Power { ON, OFF } power;
} roomba_settings_t;

typedef struct {
    enum ChargingState { NOT_CHARGING, CHARGING_RECOVERY, CHARGING, TRICKLE_CHARGING, WAITING, CHARGING_ERROR } charging_state;
    uint16_t voltage;
    int16_t current;
    int8_t temperature;
    uint16_t charge;
    uint16_t capacity;
} roomba_state_t;

bool roomba_init(int8_t dd_pin);
void roomba_update_settings(roomba_settings_t *);
void roomba_read_state(roomba_state_t *);

typedef void (*state_update_callback_fn)(roomba_state_t);
void listen_for_state_change(state_update_callback_fn callback);

#endif

#ifndef _ADVERTISE_H_
#define _ADVERTISE_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

#include "hap.h"

enum advertise_accessory_state {
    ADVERTISE_ACCESSORY_STATE_NOT_PAIRED,
    ADVERTISE_ACCESSORY_STATE_PAIRED,
};

void advertise_accessory_state(void* adv_instance);

void advertise_accessory_state_set(void* adv_instance, enum advertise_accessory_state state);

void* advertise_accessory_create(char *name, char *id, int port, uint32_t config_number,
                                 enum hap_accessory_category category, enum advertise_accessory_state state);

void advertise_accessory_remove(void* adv_instance);


bool advertise_init(char* host, char* vendor);
void advertise_terminate();


#ifdef __cplusplus
}
#endif

#endif //#ifndef _ADVERTISE_H_

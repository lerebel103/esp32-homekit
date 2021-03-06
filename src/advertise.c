#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <esp_log.h>

#include <mdns.h>
#define TAG "adv"

#include "advertise.h"

#define HAP_SERVICE "_hap"
#define HAP_PROTO "_tcp"
#define SERVICE_TXT_LEN 4

struct advertiser {
    char* name;
    char* id;
    int port;
    enum hap_accessory_category category;
    enum advertise_accessory_state state;
    uint32_t config_number;
    char service_txt_c_sharp[SERVICE_TXT_LEN];
    char service_txt_sf[SERVICE_TXT_LEN];
    char service_txt_ci[SERVICE_TXT_LEN];
};

static void _service_txt_set(struct advertiser* adv) {
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

    memset(adv->service_txt_c_sharp, 0, sizeof(adv->service_txt_c_sharp));
    sprintf(adv->service_txt_c_sharp, "%d", adv->config_number);
    memset(adv->service_txt_sf, 0, sizeof(adv->service_txt_sf));
    sprintf(adv->service_txt_sf, "%d", adv->state == ADVERTISE_ACCESSORY_STATE_NOT_PAIRED ? 1 : 0);
    memset(adv->service_txt_ci, 0, sizeof(adv->service_txt_ci));
    sprintf(adv->service_txt_ci, "%d", adv->category);

    mdns_txt_item_t hap_service_txt[] = {
        {"c#", adv->service_txt_c_sharp},
        {"ff", "0"},
        {"pv", "1.0"},
        {"id", adv->id},
        {"md", adv->name},
        {"s#", "1"},
        {"sf", adv->service_txt_sf},
        {"ci", adv->service_txt_ci},
    };

    // attempt to add service regardless
    mdns_service_add(adv->name, HAP_SERVICE, HAP_PROTO, adv->port, NULL, 0);
    mdns_service_txt_set(HAP_SERVICE, HAP_PROTO, hap_service_txt, ARRAY_SIZE(hap_service_txt));
    ESP_LOGD(TAG, "MDNS updated");
}


void advertise_accessory_state(void* adv_instance) {
    if (adv_instance == NULL) {
        ESP_LOGE(TAG, "Invalid arg, adv_instance is null in advertise_accessory_state_set");
        return;
    }

    struct advertiser* adv = adv_instance;
    _service_txt_set(adv);
}


void advertise_accessory_state_set(void* adv_instance, enum advertise_accessory_state state) {
    if (adv_instance == NULL) {
        ESP_LOGE(TAG, "Invalid arg, adv_instance is null in advertise_accessory_state_set");
        return;
    }

    struct advertiser* adv = adv_instance;
    if (adv->state == state) {
        ESP_LOGD(TAG, "Not advertising, states are identical.");
        return;
    }

    adv->state = state;
    _service_txt_set(adv);
}

void* advertise_accessory_create(char *name, char *id, int port, uint32_t config_number,
                                 enum hap_accessory_category category, enum advertise_accessory_state state) {

    if (name == NULL || id == NULL) {
        printf("[ERR] Invalid arg\n");
        return NULL;
    }
    struct advertiser* adv = calloc(1, sizeof(struct advertiser));
    if (adv == NULL) {
        printf("[ERR] calloc failed\n");
        return NULL;
    }
    ESP_LOGI(TAG, "advertise 1");

    adv->name = name;
    adv->id = id;
    adv->port = port;
    adv->config_number = config_number;
    adv->category = category;
    adv->state = state;

    return adv;
}

bool advertise_init(char* host, char* vendor) {
    bool ok = false;

    if (mdns_init() == ESP_OK) {
        if (mdns_hostname_set(host) == ESP_OK) {
            if (mdns_instance_name_set(vendor) == ESP_OK) {
                ok = true;
            } else {
                ESP_LOGE(TAG, "Could not set MDNS instance name");
                mdns_free();
            }
        } else {
            ESP_LOGE(TAG, "Could not set MDNS host name");
            mdns_free();
        }
    } else {
        ESP_LOGE(TAG, "Could not initialise MDNS");
    }

    return ok;
}

void advertise_terminate() {
    mdns_free();
}

void advertise_accessory_remove(void* adv_instance) {
    if (adv_instance == NULL)
        return;

    struct advertiser* adv = adv_instance;
    free(adv);
}

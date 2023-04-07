/**
 * Copyright (c) 2023, Iguanavation, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Neither the name of Iguanavation, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Iguanavation, Inc. "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Iguanavation, Inc. OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
 
#include <stdint.h>

#include "app_error.h"
#include "nrf_log.h"
#include "nrf_ble_scan.h"
#include "ble_advdata.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_gpio.h"

#include "ble_service.h"
 
#define    MODULE_NAME_LENGTH      7
char const module_partial_name[] = "ISS";      // Part of Module Name
uint8_t module_name[MODULE_NAME_LENGTH + 1];   // Add one for NULL terminator

#define APP_BLE_CONN_CFG_TAG            1      /**< A tag identifying the SoftDevice BLE configuration. */
#define APP_BLE_OBSERVER_PRIO           3      /**< Application's BLE observer priority. You shouldn't need to modify this value. */

NRF_BLE_SCAN_DEF(m_scan);  // Scanning module instance

void scan_start(void)
{
    ret_code_t err_code;

    err_code = nrf_ble_scan_start(&m_scan);
    APP_ERROR_CHECK(err_code);
}

void scan_init(void)
{
    ret_code_t          err_code;
    nrf_ble_scan_init_t init_scan;

    memset(&init_scan, 0, sizeof(init_scan));

    init_scan.connect_if_match = false;
    init_scan.conn_cfg_tag     = APP_BLE_CONN_CFG_TAG;

    err_code = nrf_ble_scan_init(&m_scan, &init_scan, NULL);
    APP_ERROR_CHECK(err_code);
}

static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    ret_code_t err_code;

    // For readability.
    ble_gap_evt_t const * p_gap_evt = &p_ble_evt->evt.gap_evt;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_ADV_REPORT:
    		char* result = ble_advdata_parse(p_ble_evt->evt.gap_evt.params.adv_report.data.p_data, p_ble_evt->evt.gap_evt.params.adv_report.data.len, BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME);
            if (result != NULL)
            {
                if (memcmp(result, module_partial_name, 3) == 0)
                { // match found
                    memcpy(module_name, result, 7);
                    module_name[7] = 0;  // Null Terminator
                    NRF_LOG_INFO("name: %s", module_name);
                    nrf_gpio_pin_clear(LED_PIN);
                }
            }
            break;

        default:
            // No implementation needed.
            break;
    }
}

void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}


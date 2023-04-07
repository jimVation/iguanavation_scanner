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

#include "sdk_config.h"
#include "ble_service.h"

#include "nrf_gpio.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "nrf_pwr_mgmt.h"
#include "app_timer.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

//******************************************************************************
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

//******************************************************************************
static void timer_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}

//******************************************************************************
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

//******************************************************************************
// Handle any pending log operation(s), then sleep until the next event occurs
static void idle_state_handle(void)
{
    NRF_LOG_FLUSH();
    nrf_pwr_mgmt_run();
}

//******************************************************************************
int main(void)
{
    // Initialize.
    log_init();
    timer_init();
    power_management_init();
    ble_stack_init();
    scan_init();

    // Setup LED
    nrf_gpio_cfg_output(LED_PIN);
    nrf_gpio_pin_set(LED_PIN);     // Turn LED off to start

    // Setup button
    nrf_gpio_cfg_input(OFF_BUTTON, NRF_GPIO_PIN_PULLUP);

    // Start execution.
    NRF_LOG_INFO("Iguana Bluetooth Module Scanner Started");
    scan_start();

    // Enter main loop.
    for (;;)
    {
        idle_state_handle();

        // Look for button press
        if (nrf_gpio_pin_read(OFF_BUTTON) == 0)
        {   // Turn off LED
            nrf_gpio_pin_set(LED_PIN);
        }
    }
}

/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/sensor.h>



/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   500

#define ADC_NODE DT_NODELABEL(adc)
static const struct device *adc_dev = DEVICE_DT_GET(ADC_NODE);


#define ADC_RESOLUTION 10
#define ADC_CHANNEL 0
#define ADC_CHANNEL1 1
#define ADC_CHANNEL2 2
#define ADC_CHANNEL3 3
#define ADC_PORT SAADC_CH_PSELP_PSELP_AnalogInput0
#define ADC_PORT1 SAADC_CH_PSELP_PSELP_AnalogInput1
#define ADC_PORT2 SAADC_CH_PSELP_PSELP_AnalogInput2
#define ADC_PORT3 SAADC_CH_PSELP_PSELP_AnalogInput3
#define ADC_REFERENCE ADC_REF_INTERNAL
#define ADC_GAIN ADC_GAIN_1_5

struct adc_channel_cfg channel0_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQ_TIME_DEFAULT,
	.channel_id = ADC_CHANNEL,
	.input_positive = ADC_PORT

};
struct adc_channel_cfg channel1_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQ_TIME_DEFAULT,
	.channel_id = ADC_CHANNEL1,
	.input_positive = ADC_PORT1

};
struct adc_channel_cfg channel2_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQ_TIME_DEFAULT,
	.channel_id = ADC_CHANNEL2,
	.input_positive = ADC_PORT2

};
struct adc_channel_cfg channel3_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQ_TIME_DEFAULT,
	.channel_id = ADC_CHANNEL3,
	.input_positive = ADC_PORT3

};

int16_t sample_buffer[4];

struct adc_sequence sequence = {
	.channels = BIT(ADC_CHANNEL) | BIT(ADC_CHANNEL1) | BIT(ADC_CHANNEL2) | BIT(ADC_CHANNEL3),
	.buffer = sample_buffer,
	.buffer_size = sizeof(sample_buffer),
	.resolution = ADC_RESOLUTION

};

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
//static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
	int err;
	int err2;
	if (!device_is_ready(adc_dev)) {
		printk("adc_dev not ready \n");
		return;
	}
	err = adc_channel_setup(adc_dev, &channel0_cfg);
	if (err != 0){
		printk("ADC adc_channel_setutp failed with error %d. \n", err);
		return;
	}
	err = adc_channel_setup(adc_dev, &channel1_cfg);
	if (err != 0) {
		printk("ADC adc_channel_setup failed witherror %d. \n", err);
		return;
	}
	err = adc_channel_setup(adc_dev, &channel2_cfg);
	if (err != 0) {
		printk("ADC adc_channel_setup failed witherror %d. \n", err);
		return;
	}
	err = adc_channel_setup(adc_dev, &channel3_cfg);
	if (err != 0) {
		printk("ADC adc_channel_setup failed witherror %d. \n", err);
		return;
	}

	while(1) {
	err = adc_read(adc_dev, &sequence);
	if (err != 0){
		printk("ADC adc_channel_setutp failed with error %d. \n", err);
		return;
	}
	int32_t my_value[4];
	for (int i = 0; i < 4; i++){
		my_value[i] = sample_buffer[i];
		}
	
	
	// printk("ADC Reading1: %d , ADC Reading2: %d \n", my_value, my_value[1]);
	int32_t adc_vref = adc_ref_internal(adc_dev);
	adc_raw_to_millivolts(adc_vref, ADC_GAIN, ADC_RESOLUTION, &my_value[0]);
	adc_raw_to_millivolts(adc_vref, ADC_GAIN, ADC_RESOLUTION, &my_value[1]);
	adc_raw_to_millivolts(adc_vref, ADC_GAIN, ADC_RESOLUTION, &my_value[2]);
	adc_raw_to_millivolts(adc_vref, ADC_GAIN, ADC_RESOLUTION, &my_value[3]);
	printk("ADC Voltage: %d mV, ADC Voltage 1: %d mV, ADC Voltage 2: %d mV, ADC Voltage 3: %d\n", my_value[0] , my_value[1], my_value[2], my_value[3]);
	k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

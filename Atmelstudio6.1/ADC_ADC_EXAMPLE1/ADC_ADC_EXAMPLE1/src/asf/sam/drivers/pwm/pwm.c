/**
 * \file
 *
 * \brief Pulse Width Modulation (PWM) driver for SAM.
 *
 * Copyright (c) 2011-2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "pwm.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \defgroup sam_drivers_pwm_group Pulse Width Modulation (PWM)
 *
 * See \ref sam_pwm_quickstart.
 *
 * Driver for the PWM (Pulse Width Modulation). This driver provides access to the 
 * main features of the PWM controller.
 *
 * @{
 */

#if (SAM3U || SAM3S || SAM3XA || SAM4S)
#define PWM_WRITE_PROTECT_KEY         0x50574D00
#define PWM_WRITE_PROTECT_SW_DISABLE  0
#define PWM_WRITE_PROTECT_SW_ENABLE   1
#define PWM_WRITE_PROTECT_HW_ENABLE   2
#endif

#define PWM_CLOCK_DIV_MAX  256
#define PWM_CLOCK_PRE_MAX  11

/**
 * \brief Get channel number from bitmask.
 *
 * \param ul_channel Bitmask of channel ID.
 *
 * \retval Channel number.
 */
static uint32_t pwm_converter(uint32_t ul_channel)
{
	uint32_t ul_ret = 0;

	while (!(ul_channel & 1)) {
		ul_ret++;
		ul_channel >>= 1;
	}

	return ul_ret;
}

/**
 * \brief Find a prescaler/divisor couple to generate the desired ul_frequency
 * from ul_mck.
 *
 * \param ul_frequency Desired frequency in Hz.
 * \param ul_mck Master clock frequency in Hz.
 *
 * \retval Return the value to be set in the PWM Clock Register (PWM Mode Register for SAM3N)
 * or PWM_INVALID_ARGUMENT if the configuration cannot be met.
 */
static uint32_t pwm_clocks_generate(uint32_t ul_frequency, uint32_t ul_mck)
{
	uint32_t ul_divisors[PWM_CLOCK_PRE_MAX] =
			{ 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
	uint32_t pre = 0;
	uint32_t div;

	/* Find prescaler and divisor values */
	div = (ul_mck / ul_divisors[pre]) / ul_frequency;
	while ((div > PWM_CLOCK_DIV_MAX) && (pre < PWM_CLOCK_PRE_MAX)) {
		pre++;
		div = (ul_mck / ul_divisors[pre]) / ul_frequency;
	}

	/* Return result */
	if (pre < PWM_CLOCK_PRE_MAX) {
		return div | (pre << 8);
	} else {
		return PWM_INVALID_ARGUMENT;
	}
}

/**
 * \brief Initialize the PWM source clock (clock A and clock B).
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param clock_config PWM clock configuration.
 *
 * \retval 0 if initialization succeeds, otherwise fails.
 */
uint32_t pwm_init(Pwm *p_pwm, pwm_clock_t *clock_config)
{
	uint32_t clock = 0;
	uint32_t result;

	/* Clock A */
	if (clock_config->ul_clka != 0) {
		result = pwm_clocks_generate(clock_config->ul_clka, clock_config->ul_mck);
		if (result == PWM_INVALID_ARGUMENT) {
			return result;
		}

		clock = result;
	}

	/* Clock B */
	if (clock_config->ul_clkb != 0) {
		result = pwm_clocks_generate(clock_config->ul_clkb, clock_config->ul_mck);

		if (result == PWM_INVALID_ARGUMENT) {
			return result;
		}

		clock |= (result << 16);
	}
#if SAM3N
	p_pwm->PWM_MR = clock;
#else
	p_pwm->PWM_CLK = clock;
#endif
	return 0;
}

/**
 * \brief Initialize one PWM channel.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param p_channel Configurations of the specified PWM channel.
 *
 * \retval 0 if initialization succeeds, otherwise fails.
 */
uint32_t pwm_channel_init(Pwm *p_pwm, pwm_channel_t *p_channel)
{
	uint32_t ch_mode_reg = 0;
	uint32_t channel = p_channel->channel;
	uint32_t ch_num = pwm_converter(channel);

	/* Channel Mode/Clock Register */
	ch_mode_reg = (p_channel->ul_prescaler & 0xF) |
			(p_channel->polarity << 9) |
#if (SAM3U || SAM3S || SAM3XA || SAM4S)
			(p_channel->counter_event) |
			(p_channel->b_deadtime_generator << 16) |
			(p_channel->b_pwmh_output_inverted << 17) |
			(p_channel->b_pwml_output_inverted << 18) |
#endif
			(p_channel->alignment);
	p_pwm->PWM_CH_NUM[ch_num].PWM_CMR = ch_mode_reg;

	/* Channel Duty Cycle Register */
	p_pwm->PWM_CH_NUM[ch_num].PWM_CDTY = p_channel->ul_duty;

	/* Channel Period Register */
	p_pwm->PWM_CH_NUM[ch_num].PWM_CPRD = p_channel->ul_period;

#if (SAM3U || SAM3S || SAM3XA || SAM4S)
	/* Channel Dead Time Register */
	if (p_channel->b_deadtime_generator) {
		p_pwm->PWM_CH_NUM[ch_num].PWM_DT =
				PWM_DT_DTL(p_channel->
				us_deadtime_pwml) | PWM_DT_DTH(p_channel->
				us_deadtime_pwmh);
	}

	/* Output Selection Register */
	p_pwm->PWM_OS = ((p_channel->output_selection.
					b_override_pwmh) << ch_num) |
			(((p_channel->output_selection.b_override_pwml) <<
					ch_num) << 16);

	/* Output Override Value Register */
	p_pwm->PWM_OOV = ((p_channel->output_selection.
					override_level_pwmh) << ch_num) |
			(((p_channel->output_selection.override_level_pwml) <<
					ch_num) << 16);

	/* Sync Channels Mode Register */
	if (p_channel->b_sync_ch) {
		p_pwm->PWM_SCM |= channel;
	} else {
		p_pwm->PWM_SCM &= ~((uint32_t) channel);
	}

	/* Fault Protection Value Register */
	p_pwm->PWM_FPV = ((p_channel->ul_fault_output_pwmh) << ch_num) |
			(((p_channel->ul_fault_output_pwml) << ch_num) << 16);

	/* Fault Protection Enable Register */
	uint32_t fault_enable_reg = 0;
#if (SAM3XA)
	if (ch_num < 4) {
		ch_num *= 8;
		fault_enable_reg = p_pwm->PWM_FPE1;
		fault_enable_reg &= ~(0xFF << ch_num);
		fault_enable_reg |= ((p_channel->fault_id) << ch_num);
		p_pwm->PWM_FPE1 = fault_enable_reg;
	} else {
		ch_num -= 4;
		ch_num *= 8;
		fault_enable_reg = p_pwm->PWM_FPE2;
		fault_enable_reg &= ~(0xFF << ch_num);
		fault_enable_reg |= ((p_channel->fault_id) << ch_num);
		p_pwm->PWM_FPE2 = fault_enable_reg;
	}
#endif

#if (SAM3U || SAM3S || SAM4S)
	ch_num *= 8;
	fault_enable_reg = p_pwm->PWM_FPE;
	fault_enable_reg &= ~(0xFF << ch_num);
	fault_enable_reg |= ((p_channel->fault_id) << ch_num);
	p_pwm->PWM_FPE = fault_enable_reg;
#endif
#endif

	return 0;
}

/**
 * \brief Change the period of the PWM channel.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param p_channel Configurations of the specified PWM channel.
 * \param ul_period New period value.
 *
 * \retval 0 if changing succeeds, otherwise fails.
 */
uint32_t pwm_channel_update_period(Pwm *p_pwm, pwm_channel_t *p_channel,
		uint32_t ul_period)
{
	uint32_t ch_num = pwm_converter(p_channel->channel);

	/** Check parameter */
	if (p_channel->ul_duty > ul_period) {
		return PWM_INVALID_ARGUMENT;
	} else {
		/* Save new period value */
		p_channel->ul_period = ul_period;

#if (SAM3N)
		/* Set CPD bit to change period value */
		p_pwm->PWM_CH_NUM[ch_num].PWM_CMR |= PWM_CMR_CPD;

		p_pwm->PWM_CH_NUM[ch_num].PWM_CUPD = ul_period;
#else
		p_pwm->PWM_CH_NUM[ch_num].PWM_CPRDUPD = ul_period;
#endif
	}

	return 0;
}

/**
 * \brief Change the duty cycle of the PWM channel.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param p_channel Configurations of the specified PWM channel.
 * \param ul_duty New duty cycle value.
 *
 * \retval 0 if changing succeeds, otherwise fails.
 */
uint32_t pwm_channel_update_duty(Pwm *p_pwm, pwm_channel_t *p_channel,
		uint32_t ul_duty)
{
	uint32_t ch_num = pwm_converter(p_channel->channel);

		/** Check parameter */
	if (p_channel->ul_period < ul_duty) {
		return PWM_INVALID_ARGUMENT;
	} else {
		/* Save new duty cycle value */
		p_channel->ul_duty = ul_duty;

#if (SAM3N)
		/* Clear CPD bit to change duty cycle value */
		uint32_t mode = p_pwm->PWM_CH_NUM[ch_num].PWM_CMR;
		mode &= ~PWM_CMR_CPD;
		p_pwm->PWM_CH_NUM[ch_num].PWM_CMR = mode;

		p_pwm->PWM_CH_NUM[ch_num].PWM_CUPD = ul_duty;
#else
		p_pwm->PWM_CH_NUM[ch_num].PWM_CDTYUPD = ul_duty;
#endif
	}

	return 0;
}

/**
 * \brief Return channel counter value.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param p_channel Configurations of the specified PWM channel.
 *
 * \return channel Counter value.
 */
uint32_t pwm_channel_get_counter(Pwm *p_pwm, pwm_channel_t *p_channel)
{
	uint32_t ch_num = pwm_converter(p_channel->channel);

	return p_pwm->PWM_CH_NUM[ch_num].PWM_CCNT;
}

/**
 * \brief Enable the PWM channel.
 *
 * \note The PWM channel should be initialized by pwm_channel_init() before it is enabled.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param ul_channel Bitmask of PWM channels.
 */
void pwm_channel_enable(Pwm *p_pwm, uint32_t ul_channel)
{
	p_pwm->PWM_ENA = ul_channel;
}

/**
 * \brief Disable the PWM channel.
 *
 * \note The PWM channel should be disabled, and then it can be initialized by pwm_channel_init().
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param ul_channel Bitmask of PWM channels.
 */
void pwm_channel_disable(Pwm *p_pwm, uint32_t ul_channel)
{
	p_pwm->PWM_DIS = ul_channel;
}

/**
 * \brief Check which PWM channel is enabled.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param p_channel Configurations of the specified PWM channel.
 *
 * \return Bitmask of enabled PWM channel(s).
 */
uint32_t pwm_channel_get_status(Pwm *p_pwm)
{
	return p_pwm->PWM_SR;
}

/**
 * \brief Get channel counter event and fault protection trigger interrupt status.
 *
 * \param p_pwm Pointer to a PWM instance.
 *
 * \return Channel counter event and fault protection trigger interrupt status.
 */
uint32_t pwm_channel_get_interrupt_status(Pwm *p_pwm)
{
#if (SAM3N)
	return p_pwm->PWM_ISR;
#else
	return p_pwm->PWM_ISR1;
#endif
}

/**
 * \brief Get channel counter event and fault protection trigger interrupt mask.
 *
 * \param p_pwm Pointer to a PWM instance.
 *
 * \return Channel counter event and fault protection trigger interrupt mask.
 */
uint32_t pwm_channel_get_interrupt_mask(Pwm *p_pwm)
{
#if (SAM3N)
	return p_pwm->PWM_IMR;
#else
	return p_pwm->PWM_IMR1;
#endif
}

/**
 * \brief Enable the interrupt of a channel counter event and fault protection.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param ul_event Bitmask of channel number to enable counter event interrupt.
 * \param ul_fault Bitmask of channel number to enable fault protection interrupt (ignored by SAM3N).
 */
void pwm_channel_enable_interrupt(Pwm *p_pwm, uint32_t ul_event,
		uint32_t ul_fault)
{
#if (SAM3N)
	p_pwm->PWM_IER = ul_event;
	/* Remove warning */
	ul_fault = ul_fault;
#else
	p_pwm->PWM_IER1 = ul_event | (ul_fault << 16);
#endif
}

/**
 * \brief Disable the interrupt of a channel counter event and fault protection.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param ul_event Bitmask of channel number to disable counter event interrupt.
 * \param ul_fault Bitmask of channel number to disable fault protection interrupt (ignored by SAM3N).
 */
void pwm_channel_disable_interrupt(Pwm *p_pwm, uint32_t ul_event,
		uint32_t ul_fault)
{
#if (SAM3N)
	p_pwm->PWM_IDR = ul_event;
	/* Remove warning */
	ul_fault = ul_fault;
#else
	p_pwm->PWM_IDR1 = ul_event | (ul_fault << 16);
#endif
}

#if (SAM3U || SAM3S || SAM3XA || SAM4S)
/**
 * \brief Change output selection of the PWM channel.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param p_channel Configurations of the specified PWM channel.
 * \param p_output New PWM channel output selection.
 * \param b_sync Boolean of changing of output selection. Set true to change the output synchronously (at the beginning of the next PWM period). Set false to change the output asynchronously (at the end of the execution of the function).
 */
void pwm_channel_update_output(Pwm *p_pwm, pwm_channel_t *p_channel,
		pwm_output_t *p_output, bool b_sync)
{
	uint32_t channel = p_channel->channel;
	uint32_t ch_num = pwm_converter(channel);

	bool override_pwmh = p_output->b_override_pwmh;
	bool override_pwml = p_output->b_override_pwml;
	uint32_t pwmh = p_output->override_level_pwmh;
	uint32_t pwml = p_output->override_level_pwml;

	/* Save new output configuration */
	p_channel->output_selection.b_override_pwmh = override_pwmh;
	p_channel->output_selection.b_override_pwml = override_pwml;
	p_channel->output_selection.override_level_pwmh = (pwm_level_t) pwmh;
	p_channel->output_selection.override_level_pwml = (pwm_level_t) pwml;

	/* Change override output level */
	uint32_t override_value = p_pwm->PWM_OOV;
	override_value &= ~((PWM_OOV_OOVH0 | PWM_OOV_OOVL0) << ch_num);
	override_value |= (((pwml << 16) | pwmh) << ch_num);
	p_pwm->PWM_OOV = override_value;

	/* Apply new output selection */
	if (override_pwml && override_pwmh) {
		b_sync ? (p_pwm->PWM_OSSUPD =
				(channel | (channel << 16))) : (p_pwm->PWM_OSS =
				(channel | (channel << 16)));
	} else {
		b_sync ? (p_pwm->PWM_OSCUPD =
				(channel | (channel << 16))) : (p_pwm->PWM_OSC =
				(channel | (channel << 16)));
	}
}

/**
 * \brief Change dead-time value for PWM outputs.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param p_channel Configurations of the specified PWM channel.
 * \param us_deadtime_pwmh New dead-time value for PWMH output.
 * \param us_deadtime_pwml New dead-time value for PWML output.
 */
void pwm_channel_update_dead_time(Pwm *p_pwm, pwm_channel_t *p_channel,
		uint16_t us_deadtime_pwmh, uint16_t us_deadtime_pwml)
{
	uint32_t ch_num = pwm_converter(p_channel->channel);

	/* Save new dead time value */
	p_channel->us_deadtime_pwmh = us_deadtime_pwmh;
	p_channel->us_deadtime_pwml = us_deadtime_pwml;

	/* Write channel dead time update register */
	p_pwm->PWM_CH_NUM[ch_num].PWM_DTUPD =
			PWM_DTUPD_DTLUPD(us_deadtime_pwml) |
			PWM_DTUPD_DTHUPD(us_deadtime_pwmh);
}


/**
 * \brief Initialize the behavior of a fault input.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param p_fault Configurations of a fault input.
 *
 * \retval 0 if configuration succeeds, otherwise fails.
 */
uint32_t pwm_fault_init(Pwm *p_pwm, pwm_fault_t *p_fault)
{
	uint32_t fault_id = p_fault->fault_id;
	uint32_t fault_reg = p_pwm->PWM_FMR;

	/** Polarity of fault input */
	if (p_fault->polarity == PWM_HIGH) {
		fault_reg |= fault_id;
	} else {
		fault_reg &= ~fault_id;
	}
	/** Boolean of clearing fault flag */
	if (p_fault->b_clear) {
		fault_reg |= (fault_id << 8);
	} else {
		fault_reg &= ~(fault_id << 8);
	}
	/** Boolean of fault filtering */
	if (p_fault->b_filtered) {
		fault_reg |= (fault_id << 16);
	} else {
		fault_reg &= ~(fault_id << 16);
	}

	p_pwm->PWM_FMR = fault_reg;

	return 0;
}

/**
 * \brief Get fault status.
 *
 * \param p_pwm Pointer to a PWM instance.
 *
 * \return Bitmask of IDs of all active faulty.
 */
uint32_t pwm_fault_get_status(Pwm *p_pwm)
{
	return ((p_pwm->PWM_FSR >> 8) & 0xFF);
}

/**
 * \brief Get the level of a fault input.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param id Faulty ID.
 *
 * \return Level of the fault input.
 */
pwm_level_t pwm_fault_get_input_level(Pwm *p_pwm, pwm_fault_id_t id)
{
	uint32_t fault_status_reg = p_pwm->PWM_FSR;
	fault_status_reg >>= id;

	return ((fault_status_reg & 1) ? PWM_HIGH : PWM_LOW);
}

/**
 * \brief Clear a fault input.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param id Faulty ID.
 */
void pwm_fault_clear_status(Pwm *p_pwm, pwm_fault_id_t id)
{
	p_pwm->PWM_FCR = id;
}

/**
 * \brief Initialize PWM comparison unit.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param p_cmp Configurations of PWM comparison tagged by unit.
 *
 * \retval 0 if initialization succeeds, otherwise fails.
 */
uint32_t pwm_cmp_init(Pwm *p_pwm, pwm_cmp_t *p_cmp)
{
	pmc_cmp_unit_t raw_unit = p_cmp->unit;
	uint32_t unit = pwm_converter(raw_unit);

	p_pwm->PWM_CMP[unit].PWM_CMPV = PWM_CMPV_CV(p_cmp->ul_value) |
			(p_cmp->b_is_decrementing << 24);

	p_pwm->PWM_CMP[unit].PWM_CMPM = PWM_CMPM_CTR(p_cmp->ul_trigger) |
			PWM_CMPM_CPR(p_cmp->ul_period) |
			PWM_CMPM_CUPR(p_cmp->ul_update_period);

	/** Boolean of generating a match pulse */
	if (p_cmp->b_pulse_on_line_0) {
		p_pwm->PWM_ELMR[0] |= raw_unit;
	} else {
		p_pwm->PWM_ELMR[0] &= ~((uint32_t) raw_unit);
	}
	/** Boolean of generating a match pulse */
	if (p_cmp->b_pulse_on_line_1) {
		p_pwm->PWM_ELMR[1] |= raw_unit;
	} else {
		p_pwm->PWM_ELMR[1] &= ~((uint32_t) raw_unit);
	}

	/** Boolean of comparison enable */
	if (p_cmp->b_enable) {
		p_pwm->PWM_CMP[unit].PWM_CMPM |= PWM_CMPM_CEN;
	} else {
		p_pwm->PWM_CMP[unit].PWM_CMPM &= ~PWM_CMPM_CEN;
	}

	return 0;
}

/**
 * \brief Change the setting of PWM comparison.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param p_cmp Configurations of PWM comparison tagged by unit.
 *
 * \retval 0 if changing succeeds, otherwise fails.
 */
uint32_t pwm_cmp_change_setting(Pwm *p_pwm, pwm_cmp_t *p_cmp)
{
	pmc_cmp_unit_t raw_unit = p_cmp->unit;
	uint32_t unit = pwm_converter(raw_unit);

	p_pwm->PWM_CMP[unit].PWM_CMPVUPD = PWM_CMPV_CV(p_cmp->ul_value) |
			(p_cmp->b_is_decrementing << 24);

	p_pwm->PWM_CMP[unit].PWM_CMPMUPD = PWM_CMPM_CTR(p_cmp->ul_trigger) |
			PWM_CMPM_CPR(p_cmp->ul_period) |
			PWM_CMPM_CUPR(p_cmp->ul_update_period);

	/** Boolean of generating a match pulse */
	if (p_cmp->b_pulse_on_line_0) {
		p_pwm->PWM_ELMR[0] |= raw_unit;
	} else {
		p_pwm->PWM_ELMR[0] &= ~((uint32_t) raw_unit);
	}
	/** Boolean of generating a match pulse */
	if (p_cmp->b_pulse_on_line_1) {
		p_pwm->PWM_ELMR[1] |= raw_unit;
	} else {
		p_pwm->PWM_ELMR[1] &= ~((uint32_t) raw_unit);
	}

	/** Boolean of comparison enable */
	if (p_cmp->b_enable) {
		p_pwm->PWM_CMP[unit].PWM_CMPMUPD |= PWM_CMPM_CEN;
	} else {
		p_pwm->PWM_CMP[unit].PWM_CMPMUPD &= ~PWM_CMPM_CEN;
	}

	return 0;
}

/**
 * \brief Report the value of the comparison period counter.
 *
 * \param p_pwm	Pointer to a PWM instance.
 * \param unit PWM comparison unit.
 *
 * \return Value of the comparison period counter.
 */
uint32_t pwm_cmp_get_period_counter(Pwm *p_pwm, pmc_cmp_unit_t unit)
{
	uint32_t i = pwm_converter(unit);
	uint32_t counter = PWM_CMPM_CPRCNT(p_pwm->PWM_CMP[i].PWM_CMPM);

	return (counter >> PWM_CMPM_CPRCNT_Pos);
}

/**
 * \brief Report the value of the comparison update period counter.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param unit PWM comparison unit.
 *
 * \return Value of the comparison update period counter.
 */
uint32_t pwm_cmp_get_update_counter(Pwm *p_pwm, pmc_cmp_unit_t unit)
{
	uint32_t i = pwm_converter(unit);
	uint32_t counter = PWM_CMPM_CUPRCNT(p_pwm->PWM_CMP[i].PWM_CMPM);

	return (counter >> PWM_CMPM_CUPRCNT_Pos);
}

/**
 * \brief Enable the interrupt of comparison.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param ul_sources Bitmask of comparison unit.
 * \param type PWM_CMP_MATCH enables the match interrupt of the unit. PWM_CMP_UPDATE enables the update interrupt of the comparison unit.
 */
void pwm_cmp_enable_interrupt(Pwm *p_pwm, uint32_t ul_sources,
		pwm_cmp_interrupt_t type)
{
	if (type == PWM_CMP_MATCH) {
		p_pwm->PWM_IER2 = (ul_sources << 8);
	} else if (type == PWM_CMP_UPDATE) {
		p_pwm->PWM_IER2 = (ul_sources << 16);
	} else {
		/* Do Nothing */
	}
}

/**
 * \brief Disable the interrupt of comparison.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param ul_sources Bitmask of comparison unit.
 * \param type PWM_CMP_MATCH disables the match interrupt of the unit. PWM_CMP_UPDATE disables the update interrupt of the comparison unit.
 */
void pwm_cmp_disable_interrupt(Pwm *p_pwm, uint32_t ul_sources,
		pwm_cmp_interrupt_t type)
{
	if (type == PWM_CMP_MATCH) {
		p_pwm->PWM_IDR2 = (ul_sources << 8);
	} else if (type == PWM_CMP_UPDATE) {
		p_pwm->PWM_IDR2 = (ul_sources << 16);
	} else {
		/* Do Nothing */
	}
}

/**
 * \brief Set PDC transfer request mode.
 *
 * \note If configure Synchronous channels update mode as 'PWM_SYNC_UPDATE_MODE_0' or 'PWM_SYNC_UPDATE_MODE_1' via pwm_sync_init(), ul_pdc_request will be ignored and PDC transfer request will never occur.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param request_mode PDC transfer request mode.
 * \param cmp_unit The comparison unit of PDC transfer request.
 */
void pwm_pdc_set_request_mode(Pwm *p_pwm, pwm_pdc_request_mode_t request_mode,
		pmc_cmp_unit_t cmp_unit)
{
	uint32_t unit = pwm_converter(cmp_unit);
	uint32_t sync_mode = p_pwm->PWM_SCM;

	sync_mode &= ~(PWM_SCM_PTRCS_Msk | PWM_SCM_PTRM);
	sync_mode |= (PWM_SCM_PTRCS(unit) | request_mode);

	p_pwm->PWM_SCM = sync_mode;
}

/**
 * \brief Enable the interrupt of PDC transfer.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param ul_sources Bitmask of PWM PDC transfer interrupt sources.
 */
void pwm_pdc_enable_interrupt(Pwm *p_pwm, uint32_t ul_sources)
{
	p_pwm->PWM_IER2 = ul_sources;
}

/**
 * \brief Disable the interrupt of PDC transfer.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param ul_sources Bitmask of PWM PDC transfer interrupt sources.
 */
void pwm_pdc_disable_interrupt(Pwm *p_pwm, uint32_t ul_sources)
{
	p_pwm->PWM_IDR2 = ul_sources;
}

/**
 * \brief Initialize synchronous channels update mode and period.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param mode Synchronous channels update mode.
 * \param ul_update_period Time between each update of the synchronous channels.
 *
 * \retval 0 if initialization succeeds, otherwise fails.
 */
uint32_t pwm_sync_init(Pwm *p_pwm, pwm_sync_update_mode_t mode,
		uint32_t ul_update_period)
{
	uint32_t sync_mode = p_pwm->PWM_SCM;

	sync_mode &= ~PWM_SCM_UPDM_Msk;
	sync_mode |= mode;

	p_pwm->PWM_SCM = sync_mode;

	p_pwm->PWM_SCUP = PWM_SCUP_UPR(ul_update_period);

	return 0;
}

/**
 * \brief Unlock the update of synchronous channels.
 *
 * \note After unlock, synchronous channels will be updated at the next PWM period.
 *
 * \param p_pwm Pointer to a PWM instance.
 */
void pwm_sync_unlock_update(Pwm *p_pwm)
{
	p_pwm->PWM_SCUC = PWM_SCUC_UPDULOCK;
}

/**
 * \brief Change the wanted time between each update of the synchronous channels.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param ul_update_period Time between each update of the synchronous channels.
 */
void pwm_sync_change_period(Pwm *p_pwm, uint32_t ul_update_period)
{
	p_pwm->PWM_SCUPUPD = PWM_SCUPUPD_UPRUPD(ul_update_period);
}

/**
 * \brief Get the value of the synchronization update period counter.
 *
 * \param p_pwm Pointer to a PWM instance.
 *
 * \return Value of the synchronization update Period Counter.
 */
uint32_t pwm_sync_get_period_counter(Pwm *p_pwm)
{
	return PWM_SCUP_UPRCNT(p_pwm->PWM_SCUP);
}

/**
 * \brief Enable the interrupt of synchronous channel.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param ul_sources Bitmask of PWM synchronous channels interrupt sources.
 */
void pwm_sync_enable_interrupt(Pwm *p_pwm, uint32_t ul_sources)
{
	p_pwm->PWM_IER2 = ul_sources;
}

/**
 * \brief Disable the interrupt of synchronous channels.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param ul_sources Bitmask of PWM synchronous channels interrupt sources.
 */
void pwm_sync_disable_interrupt(Pwm *p_pwm, uint32_t ul_sources)
{
	p_pwm->PWM_IDR2 = ul_sources;
}

/**
 * \brief Enable PWM write protect.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param ul_group Bitmask of PWM register group.
 * \param b_sw Boolean of software protect. True for software protect and false for hardware protect.
 */
void pwm_enable_protect(Pwm *p_pwm, uint32_t ul_group, bool b_sw)
{
	uint32_t wp = 0;

	if (b_sw) {
		wp = PWM_WRITE_PROTECT_KEY | (ul_group << 2) |
				PWM_WPCR_WPCMD(PWM_WRITE_PROTECT_SW_ENABLE);
	} else {
		wp = PWM_WRITE_PROTECT_KEY | (ul_group << 2) |
				PWM_WPCR_WPCMD(PWM_WRITE_PROTECT_HW_ENABLE);
	}

	p_pwm->PWM_WPCR = wp;
}

/**
 * \brief Disable PWM write protect.
 *
 * \note Only a hardware reset of PWM controller (handled by PMC) can disable hardware write protect.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param ul_group Bitmask of PWM register group.
 */
void pwm_disable_protect(Pwm *p_pwm, uint32_t ul_group)
{
	p_pwm->PWM_WPCR =
			PWM_WRITE_PROTECT_KEY | (ul_group << 2) |
			PWM_WPCR_WPCMD(PWM_WRITE_PROTECT_SW_DISABLE);
}

/**
 * \brief Get PWM write protect status.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param p_protect Pointer to a structure stored PWM protect status.
 *
 * \retval 0 Protection disabled.
 * \retval 1 Protection enabled.
 */
bool pwm_get_protect_status(Pwm *p_pwm, pwm_protect_t *p_protect)
{
	uint32_t wpsr = p_pwm->PWM_WPSR;

	p_protect->ul_hw_status = (wpsr >> 8) & 0x3F;
	/** Bitmask of PWM register group for write protect software status */
	p_protect->ul_sw_status = (wpsr & 0x3F);

	if ((PWM_WPSR_WPVS & wpsr) == PWM_WPSR_WPVS) {
		p_protect->ul_offset =
				(wpsr & PWM_WPSR_WPVSRC_Msk) >>
				PWM_WPSR_WPVSRC_Pos;
		return true;
	} else {
		return false;
	}
}

/**
 * \brief Get interrupt status of PDC transfer, synchronous channels and comparison.
 *
 * \param p_pwm Pointer to a PWM instance.
 *
 * \return Interrupt status of PDC transfer, synchronous channels and comparison.
 */
uint32_t pwm_get_interrupt_status(Pwm *p_pwm)
{
	return p_pwm->PWM_ISR2;
}

/**
 * \brief Get interrupt mask of PDC transfer, synchronous channels and comparison.
 *
 * \param p_pwm Pointer to a PWM instance.
 *
 * \return Interrupt mask of PDC transfer, synchronous channels and comparison.
 */
uint32_t pwm_get_interrupt_mask(Pwm *p_pwm)
{
	return p_pwm->PWM_IMR2;
}
#endif

#if (SAM3S || SAM3XA || SAM4S)
/**
 * \brief Initialize PWM stepper motor mode.
 *
 * \param p_pwm Pointer to a PWM instance.
 * \param pair PWM channels used by stepper motor.
 * \param b_enable_gray Set true to enable gray count generation. Set false to disable it.
 * \param b_down Set true to use down counter. Set false to use up counter.
 */
void pwm_stepper_motor_init(Pwm *p_pwm, pwm_stepper_motor_pair_t pair,
		bool b_enable_gray, bool b_down)
{
	uint32_t motor = p_pwm->PWM_SMMR;

	motor &= ((PWM_SMMR_GCEN0 | PWM_SMMR_DOWN0) << pair);
	motor |= ((b_enable_gray | (b_down << 16)) << pair);

	p_pwm->PWM_SMMR = motor;
}
#endif

//@}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

#include "drv_lis2dh.h"
#include "twi_master.h"
#include "nrf_delay.h"

#include "app_trace.h"

int LIS2DH_G;

static int drv_lis2dh_read_reg_range(uint8_t reg, uint8_t value)
{
	uint8_t payload[] = { reg, value };

	return !twi_master_transfer(LIS2DH_ADDR, payload, 2, TWI_ISSUE_STOP);
}

static int read_reg_range(uint8_t start, uint8_t* value, int len)
{
	bool good;

	start |= 0x80;

	good  = twi_master_transfer(LIS2DH_ADDR, &start, 1, TWI_DONT_ISSUE_STOP);
	good &= twi_master_transfer(LIS2DH_ADDR | TWI_READ_BIT, value, len, TWI_ISSUE_STOP);

	return !good;
}

lis2dh_cfg_t drv_lis2dh_default_cfg()
{
	const static lis2dh_cfg_t defult_cfg = {
		.ctrl_regs = {
			LIS2DH_REG1_10HZ | LIS2DH_REG1_X_AXIS | LIS2DH_REG1_Y_AXIS | LIS2DH_REG1_Z_AXIS, // REG1
			0, // REG2
			LIS2DH_REG3_AOI1_INT1, // REG3
			LIS2DH_REG4_BDU_READ | LIS2DH_REG4_SCL_8G, // REG4
			0,  // REG5
			0, // REG6
		},
		.fifo_reg = LIS2DH_FIFO_BYPASS,
		.sleep_duration = 30,
		.sleep_threshold = LIS2DH_CO_8G * 0.1,
		.ints = {
			{ 
				.reg = LIS2DH_INT_AND_EVTS | LIS2DH_INT_Z_LO | LIS2DH_INT_X_LO,
				.threshold = 0.2 * LIS2DH_CO_8G, // 62mg acceleration
				.duration = 1   // 0.2 second duration
			},
		},
	};

	return defult_cfg;
}

int drv_lis2dh_cfg(lis2dh_cfg_t* cfg)
{
	printf("\r\n[drv_lis2dh]\r\n");

	if(!cfg)
	{
		return -1;
	}

	uint8_t who = 0;
	read_reg_range(LIS2DH_WHO_AM_I, &who, 1);
	if(who != 0x33)
		return -40; // Device verification failed

	// trigger a reboot
	drv_lis2dh_read_reg_range(LIS2DH_REG5, LIS2DH_REG5_REBOOT);
	nrf_delay_ms(5);

	// Set control registers
	const uint8_t ctrl_reg_start = LIS2DH_REG1;
	printf("CTRL_REG: ");
	for(int i = 0; i < 6; ++i)
	{
		printf("%02x ", cfg->ctrl_regs[i]);

		if(drv_lis2dh_read_reg_range(ctrl_reg_start + i, cfg->ctrl_regs[i]))
		{
			return -(i + 1); // indicate reg that triggered failure
		}
		nrf_delay_ms(1);
	}
	printf("\r\n");

	// write interrupt configurations
	static const int int_pins[2] = { NRF51_INT1_PIN, NRF51_INT2_PIN };
	static const uint8_t reg_addrs[] = {
		LIS2DH_INT1_CFG_REG,
		LIS2DH_INT1_THRESHHOLD_REG,
		LIS2DH_INT1_DURATION_REG,
	};

	for(int int_i = 0; int_i < 2; ++int_i)
	{
		printf("INT%d ", int_i);
		uint8_t int_base = int_i * 4;
		for(int i = 0; i < 3; ++i)
		{
			uint8_t reg = reg_addrs[i] + int_base;
			uint8_t val = ((uint8_t*)(cfg->ints + int_i))[i];
			printf("%02x ", val);
			if(drv_lis2dh_drv_lis2dh_read_reg_range(reg, val))
			{
				return -(i + int_base); // indicate reg that triggered failure
			}
		}
		printf("\r\n");


	}

	if(drv_lis2dh_read_reg_range(LIS2DH_FIFO_CTRL_REG, cfg->fifo_reg))
		return -10; // fifo reg failure

	if(drv_lis2dh_read_reg_range(LIS2DH_SLEEP_THRESHOLD_REG, cfg->sleep_threshold))
		return -20; // sleep threshold failure

	if(drv_lis2dh_read_reg_range(LIS2DH_SLEEP_DURATION_REG, cfg->sleep_duration))
		return -30; // sleep duration failure

	// Set G to the integer value that corresponds to a reading of 1G
	switch (cfg->ctrl_regs[3] & LIS2DH_REG4_SCL_MSK) {
		case LIS2DH_REG4_SCL_2G:
			LIS2DH_G = 1 / 0.016;
			break;
		case LIS2DH_REG4_SCL_4G:
			LIS2DH_G = 1 / 0.032;
			break;
		case LIS2DH_REG4_SCL_8G:
			LIS2DH_G = 1 / 0.062;
			break;
		case LIS2DH_REG4_SCL_16G:
			LIS2DH_G = 1 / 0.186;
			break;
	}

	// clear interrupts
	drv_lis2dh_int_src(0);
	drv_lis2dh_int_src(1);

	return 0; // all good
}

int drv_lis2dh_get_acc(vec3_t* v, unsigned int samples)
{
	for(int i = 0; i < samples; ++i)
	{
#ifdef VEC_16
		read_reg_range(LIS2DH_OUT_X_L, (uint8_t*)v[i].v, sizeof(VEC_TYPE) * 3);
		v->x >>= 8;
		v->y >>= 8;
		v->z >>= 8;
#else
		read_reg_range(LIS2DH_OUT_X_L + 1, (uint8_t*)&v[i].x, sizeof(VEC_TYPE));
		read_reg_range(LIS2DH_OUT_Y_L + 1, (uint8_t*)&v[i].y, sizeof(VEC_TYPE));
		read_reg_range(LIS2DH_OUT_Z_L + 1, (uint8_t*)&v[i].z, sizeof(VEC_TYPE));
#endif
	}

	return 0;
}

uint8_t drv_lis2dh_int_src(uint8_t i)
{
	const uint8_t regs[] = { LIS2DH_INT1_SRC_REG, LIS2DH_INT2_SRC_REG };
	uint8_t val = 0;

	read_reg_range(regs[i], &val, 1);

	return val;
}

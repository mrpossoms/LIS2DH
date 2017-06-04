/**
 *
 * MIT License
 *
 * Copyright (c) 2017 Kirk Roerig
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ----------------------------------------------------------------------------
 *  File: drv_lis2dh.h
 *  Author: Kirk Roerig (mr.possoms@gmail.com)
 *  Description: This file implements a driver for the ST Micro LIS2DH
 *               accelerometer. Detailed information about the device can
 *               be found at (http://www.st.com/en/mems-and-sensors/lis2dh.html) .
 */

#ifndef DRV_LIS2DH
#define DRV_LIS2DH

#define VEC_16

#ifdef VEC_16
#define VEC_TYPE int16_t
#else
#define VEC_TYPE int8_t
#endif

// Coefficents for scaling from units of G to integer thresholds for each scale
#define LIS2DH_CO_2G  ( 1.f / 0.016f )
#define LIS2DH_CO_4G  ( 1.f / 0.032f )
#define LIS2DH_CO_8G  ( 1.f / 0.062f )
#define LIS2DH_CO_16G ( 1.f / 0.186f )

#define LIS2DH_ADDR                0x30

#define LIS2DH_WHO_AM_I            0x0F

#define LIS2DH_REG1                0x20
#define LIS2DH_REG1_POWER_DOWN     0x00
#define LIS2DH_REG1_1HZ            0x10
#define LIS2DH_REG1_10HZ           0x20
#define LIS2DH_REG1_25HZ           0x30
#define LIS2DH_REG1_50HZ           0x40
#define LIS2DH_REG1_100HZ          0x50
#define LIS2DH_REG1_200HZ          0x60
#define LIS2DH_REG1_400HZ          0x70
#define LIS2DH_REG1_LOW_POWER      0x08
#define LIS2DH_REG1_Z_AXIS         0x04
#define LIS2DH_REG1_Y_AXIS         0x02
#define LIS2DH_REG1_X_AXIS         0x01
#define LIS2DH_REG1_ALL_AXES       0x07

#define LIS2DH_REG2                0x21
#define LIS2DH_REG2_HPF_NORMAL     0x00
#define LIS2DH_REG2_HPF_REF_SIG    0x40
#define LIS2DH_REG2_HPF_INT_RESET  0xC0
#define LIS2DH_REG2_HPF_USE        0x08
#define LIS2DH_REG2_HPF_CLICK      0x04
#define LIS2DH_REG2_HPF_AOI_INT2   0x02    // AOI - And Or Interrupt events
#define LIS2DH_REG2_HPF_AOI_INT1   0x01

#define LIS2DH_REG3                0x22
#define LIS2DH_REG3_CLICK_INT1     0x80
#define LIS2DH_REG3_AOI1_INT1      0x40
#define LIS2DH_REG3_AOI2_INT1      0x20
#define LIS2DH_REG3_DRDY1_INT1     0x10
#define LIS2DH_REG3_DRDY2_INT1     0x08
#define LIS2DH_REG3_FIFO_WTR_INT1  0x04
#define LIS2DH_REG3_FIFO_OVR_INT1  0x02

#define LIS2DH_REG4                0x23
#define LIS2DH_REG4_BDU_CONT       0x00    // Updates are continuous
#define LIS2DH_REG4_BDU_READ       0x80    // Updates after read
#define LIS2DH_REG4_LITTLE_ENDIAN  0x00
#define LIS2DH_REG4_BIG_ENDIAN     0x40
#define LIS2DH_REG4_HI_RES         0x08
#define LIS2DH_REG4_SCL_2G         0x00
#define LIS2DH_REG4_SCL_4G         0x10
#define LIS2DH_REG4_SCL_8G         0x20
#define LIS2DH_REG4_SCL_16G        0x30
#define LIS2DH_REG4_SCL_MSK        0x30

#define LIS2DH_REG5                0x24
#define LIS2DH_REG5_REBOOT         0x80
#define LIS2DH_REG5_USE_FIFO       0x40
#define LIS2DH_REG5_LIR_INT1       0x08
#define LIS2DH_REG5_D4D_INT1       0x04
#define LIS2DH_REG5_LIR_INT2       0x02
#define LIS2DH_REG5_D4D_INT2       0x01

#define LIS2DH_REG6                0x25
#define LIS2DH_REG6_CLICK_INT2     0x80
#define LIS2DH_REG6_INT1_FUNC_INT2 0x40
#define LIS2DH_REG6_INT2_FUNC_INT2 0x20
#define LIS2DH_REG6_BOOT_INT2      0x10
#define LIS2DH_REG6_ACTIVITY_INT2  0x08
#define LIS2DH_REG6_INT_ACT_LOW    0x02

#define LIS2DH_INT_REFERENCE       0x26

#define LIS2DH_FIFO_CTRL_REG       0x2E
#define LIS2DH_FIFO_BYPASS         0x00
#define LIS2DH_FIFO_FIFO           0x40
#define LIS2DH_FIFO_STREAM         0x80
#define LIS2DH_FIFO_TRIGGER        0xC0

#define LIS2DH_INT1_CFG_REG        0x30
#define LIS2DH_INT1_SRC_REG        0x31
#define LIS2DH_INT1_THRESHHOLD_REG 0x32
#define LIS2DH_INT1_DURATION_REG   0x33    // 1 LSb = 1/ODR

#define LIS2DH_INT2_CFG_REG        0x34
#define LIS2DH_INT2_SRC_REG        0x35
#define LIS2DH_INT2_THRESHHOLD_REG 0x36
#define LIS2DH_INT2_DURATION_REG   0x37    // 1 LSb = 1/ODR

#define LIS2DH_SLEEP_THRESHOLD_REG 0x3E
#define LIS2DH_SLEEP_DURATION_REG  0x3F    // 1LSb = (8*1[LSb]+1)/ODR

// Usable with INT1 and INT2 cfg registers
#define LIS2DH_INT_OR_EVTS         0x00
#define LIS2DH_INT_AND_EVTS        0x80
#define LIS2DH_INT_MOVEMENT        0x40
#define LIS2DH_INT_DIRECTION       0xC0
#define LIS2DH_INT_Z_HI            0x20
#define LIS2DH_INT_Z_LO            0x10
#define LIS2DH_INT_Y_HI            0x08
#define LIS2DH_INT_Y_LO            0x04
#define LIS2DH_INT_X_HI            0x02
#define LIS2DH_INT_X_LO            0x01

#define LIS2DH_OUT_X_L             0x28
#define LIS2DH_OUT_Y_L             0x2A
#define LIS2DH_OUT_Z_L             0x2C

#define NRF51_INT1_PIN             20
#define NRF51_INT2_PIN             11

typedef struct {
	uint8_t reg;
	uint8_t threshold;
	uint8_t duration;
} lis2dh_int_cfg_t;

typedef struct {
	uint8_t ctrl_regs[6];
	uint8_t fifo_reg;
	uint8_t sleep_duration;
	uint8_t sleep_threshold;
	lis2dh_int_cfg_t ints[2];
} lis2dh_cfg_t;

typedef union {
	struct {
		VEC_TYPE x, y, z;
	};
	VEC_TYPE v[3];
} vec3_t;

// Integer value on an axis when exactly 1G is exerted on the unit
extern int LIS2DH_G;

/** \brief Writes a single byte to the device
 *  \param reg   - Device's register address to which value will be written.
 *  \param value - Byte value which will be written to reg
 *  \return Non-zero on error
 *  
 *  This functino must be implemented by you. It should transmit one byte
 *  to the LIS2DH using either SPI or I2C. The function should also
 *  address the device appropriately based on its' configuration.
 */
extern int drv_lis2dh_write_reg(uint8_t reg, uint8_t value);

/** \brief Reads an array of registers from the device
 *  \param start - Register to start reading at
 *  \param value - Pointer to destination array
 *  \param len   - Bytes to read from the device.
 *  \return Non-zero on error
 *
 *  This function must be implemented by you. It should read 'len' bytes
 *  from the LIS2DH using either SPI or I2C. The function should also
 *  address the device appropriately based on its' configuration.
 */
extern int drv_lis2dh_read_reg_range(uint8_t start, uint8_t* value, int len);

lis2dh_cfg_t drv_lis2dh_default_cfg();
int          drv_lis2dh_cfg(lis2dh_cfg_t* cfg);
int          drv_lis2dh_get_acc(vec3_t* v, unsigned int samples);
uint8_t      drv_lis2dh_int_src(uint8_t i); // 0 indexed

#endif

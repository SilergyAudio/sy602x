/*

sy6026l.c

note: if any statement starts with " /* ", you need change with your own program
Chinese Note: [注释：任何以 "/*" 注释掉的语句，都要你们来补充相应的程序]

if any questions, pls contact email hejinsheng.cao@silergycorp.com
                      or phone number: 18368841398
					  
Silergy Corp. Reserved

*/

#include <linux/i2c/sy6026l.h>

// you may define SY6026L_SLAVE_ADDR in sy6026l.h instead
#define SY6026L_SLAVE_ADDR 0x2A // ADDR_SEL pin is low (default): for 7bit is 0x2A (and 8bit 0x54); 
								// if ADDR_SEL pin is high: for 7bit is 0x2B (and 8bit 0x56);

static int sy6026l_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	/* you need add other probe configurations here */
	
	
	
	
	
	

	
	// **** SY6026L initial configuration ****
	/* 1. pin configuration
	      a) make sure ADDR_SEL pin (pin31) is low or high. if low, 7bit slave address is 0x2A; and if high, 7bit slave address is 0x2B
	      b) confirm that RST_B pin (pin30) is low for normal operation; RST_B pin is a reset pin, reset the whole system (including I2C register values)
		  c) confirm that FAULT_B pin (pin10) is high for normal operation; FAULT_B pin is a enable pin, enable power stage output
	   2. delay 20ms after RST_B from low to high before I2C configuration
	   3. you need choose one of the below functions to initial SY6026L  */
	sy6026l_initial_simple_setting(client); // initial program for SY6026L
	//sy6026l_initial_full_setting(client); // initial program for SY6026L
	
	/* return ret; */
}

static int sy6026l_nbytes_write(struct i2c_client *client,
		unsigned int reg, const long values[], size_t len)
{
	/* you need write these program */
	/* I2C multi-bytes write function */
	
	
	
	
	
}

static int sy6026l_byte_write(struct i2c_client *client, u8 reg, u8 val)
{
	/* you need write these program */
	/* I2C single byte write function */
	
	
	
	
	
}






// *************************************************************************************
//  below is the initial functions and I2C setting functions for SY6026L
// *************************************************************************************



void sy6026l_initial_simple_setting(struct i2c_client *client) // very simple initial program after power up
{
	sy6026l_byte_write(client, 0x22, 0x01);	// enable power stage from HIZ to PWM out; initial 31; 01-->ternary mode, 00-->BD mode
	sy6026l_byte_write(client, 0x07, 0xff);	// adjust volume from mute to 0dB; initial 00
											// you need adjust it to a correct volume you want;
											// here 0dB is the biggest volume 0x07 register can set
}

void sy6026l_initial_full_setting(struct i2c_client *client) // attention: full initial program for first time after power up
{
	// **** pwmstage setting for better performance ****
	sy6026l_pwmstage_write(client); 
	
	// **** fault & recovery setting for protection ****
	sy6026l_faultfunction_write(client);
	
	// **** input mixer setting, use when output are a sub-woofer + a high_freq speaker configuration ****
	// sy6026l_mixer_write(client);	
	
	// **** EQ and DRC setting *****
	// sy6026l_before_eq_drc_write(client) // to avoid pop noise when eq/drc setting; do not need at first time setting
	sy6026l_eq_write(client); // eq parameter setting
	sy6026l_drc_write(client); // drc paras setting
	// sy6026l_after_eq_drc_write(client) // to recover to normal state after eq/drc setting; do not need at first time setting
	sy6026l_byte_write(client, 0x03, 0x18);	// disable I2C access to RAM and enable DAP(audio processor) access to RAM
	
	// **** enable output & volume control *****
	sy6026l_byte_write(client, 0x22, 0x01);	// enable power stage from HIZ to PWM out; initial 31; 01-->ternary mode, 00-->BD mode
	sy6026l_byte_write(client, 0x07, 0xff);	// adjust volume from mute to 0dB(ff); initial 00; 
											// you need adjust it to a correct volume you want;
											// here 0dB is the biggest volume 0x07 register can set
}

void sy6026l_pwmstage_write(struct i2c_client *client) // power stage setting
{
	sy6026l_byte_write(client, 0x11, 0x80);	//	pwm phase delay for better SNR
	sy6026l_byte_write(client, 0x12, 0x80);	//	pwm phase delay for better SNR
	sy6026l_byte_write(client, 0x13, 0x7C);	//	pwm phase delay for better SNR
	sy6026l_byte_write(client, 0x14, 0x7C);	//	pwm phase delay for better SNR
	
	sy6026l_nbytes_write(client, 0x55, sy6026l_reg_outflip_55, 4);	//	minimum pwm width limit enable 
	sy6026l_nbytes_write(client, 0x56, sy6026l_reg_pwmctrl_56, 4);	//  dvalue for better performance; use for ternary mode (0x22=0x01)
}

void sy6026l_faultfunction_write(struct i2c_client *client) // fault recovery function setting
{
	// for more fault function or other fault recovery setting, pls refer to register 0x1B & 0x23
	sy6026l_byte_write(client, 0x1B, 0x1D);	//	short control: retry interval: 1s; fault clear intercal: 1ms
	sy6026l_byte_write(client, 0x23, 0x00);	//	short control: Fault detected 2 times, lock fault
}

void sy6026l_mixer_write(struct i2c_client *client) // input mixer setting
{
	// for more mixer function or other mixer setting, pls refer to register 0x5f
	sy6026l_nbytes_write(client, 0x5F, sy6026l_reg_mixer_5f, 4);	//	mixer gain setting: out_ch1 = out_ch2 = 0.5*in_ch1+0.5*in_ch2
}

void sy6026l_eq_change(struct i2c_client *client) // when playing music and want to change eq setting
{
	sy6026l_before_eq_drc_write(client); // to avoid pop noise when eq/drc setting
	sy6026l_eq_write(client); // eq parameter setting
	sy6026l_after_eq_drc_write(client); // to recover to normal state after eq/drc setting
}

void sy6026l_drc_change(struct i2c_client *client) // when playing music and want to change drc setting
{
	sy6026l_before_eq_drc_write(client); // to avoid pop noise when eq/drc setting
	sy6026l_drc_write(client); // drc paras setting
	sy6026l_after_eq_drc_write(client); // to recover to normal state after eq/drc setting
}

void sy6026l_before_eq_drc_write(struct i2c_client *client) // before eq/drc change
{
	sy6026l_byte_write(client, 0x06, 0x08); // soft mute 
	delay_ms(20); /* need you add a 20ms delay function; 20ms delay time is for soft mute time */
	sy6026l_byte_write(client, 0x22, 0x31); // shutdown power stage
	sy6026l_byte_write(client, 0x03, 0x19); // enable I2C access to RAM (register address: 0x30->0x3a)
}

void sy6026l_after_eq_drc_write(struct i2c_client *client) // after eq/drc change
{
	sy6026l_byte_write(client, 0x03, 0x18);	// disable I2C access to RAM and enable DAP(audio processor) access to RAM
	sy6026l_byte_write(client, 0x22, 0x01); // enable power stage; if 0x22=0x31 before sy6026l_before_eq_drc_write() function, then keep 22=31 and ignore this program
	sy6026l_byte_write(client, 0x06, 0x10); // unmute
}

//	EQ write
void sy6026l_eq_write(struct i2c_client *client)
{
	sy6026l_nbytes_write(client, 0x30, sy6026l_reg_eq_30, 20);
	sy6026l_nbytes_write(client, 0x31, sy6026l_reg_eq_31, 20);
	sy6026l_nbytes_write(client, 0x32, sy6026l_reg_eq_32, 20);
	sy6026l_nbytes_write(client, 0x33, sy6026l_reg_eq_33, 20);
	sy6026l_nbytes_write(client, 0x34, sy6026l_reg_eq_34, 20);
	sy6026l_nbytes_write(client, 0x35, sy6026l_reg_eq_35, 20);
	sy6026l_nbytes_write(client, 0x37, sy6026l_reg_eq_37, 20);
	sy6026l_nbytes_write(client, 0x38, sy6026l_reg_eq_38, 20);
	sy6026l_nbytes_write(client, 0x39, sy6026l_reg_eq_39, 20);
	sy6026l_nbytes_write(client, 0x3a, sy6026l_reg_eq_3a, 20);
}

//	drc write
void sy6026l_drc_write(struct i2c_client *client)
{
	sy6026l_nbytes_write(client, 0x47, sy6026l_reg_drc_47, 20);
	sy6026l_nbytes_write(client, 0x49, sy6026l_reg_drc_49, 20);
	sy6026l_nbytes_write(client, 0x4a, sy6026l_reg_drc_4a, 20);
	sy6026l_nbytes_write(client, 0x4c, sy6026l_reg_drc_4c, 20);
	sy6026l_nbytes_write(client, 0x4e, sy6026l_reg_drc_4e, 3);
	sy6026l_nbytes_write(client, 0x4f, sy6026l_reg_drc_4f, 3);
	sy6026l_nbytes_write(client, 0x50, sy6026l_reg_drc_50, 3);
	sy6026l_nbytes_write(client, 0x51, sy6026l_reg_drc_51, 3);
	sy6026l_nbytes_write(client, 0x52, sy6026l_reg_drc_52, 3);
	sy6026l_nbytes_write(client, 0x53, sy6026l_reg_drc_53, 3);
	sy6026l_nbytes_write(client, 0x60, sy6026l_reg_drc_60, 4);
	sy6026l_nbytes_write(client, 0x61, sy6026l_reg_drc_61, 3);
	sy6026l_nbytes_write(client, 0x64, sy6026l_reg_drc_64, 3);
	sy6026l_nbytes_write(client, 0x67, sy6026l_reg_drc_67, 3);
	sy6026l_nbytes_write(client, 0x6a, sy6026l_reg_drc_6a, 3);
	sy6026l_nbytes_write(client, 0x62, sy6026l_reg_drc_62, 3);
	sy6026l_nbytes_write(client, 0x65, sy6026l_reg_drc_65, 3);
	sy6026l_nbytes_write(client, 0x68, sy6026l_reg_drc_68, 3);
	sy6026l_nbytes_write(client, 0x6b, sy6026l_reg_drc_6b, 3);
	sy6026l_nbytes_write(client, 0x63, sy6026l_reg_drc_63, 3);
	sy6026l_nbytes_write(client, 0x66, sy6026l_reg_drc_66, 3);
	sy6026l_nbytes_write(client, 0x69, sy6026l_reg_drc_69, 3);
	sy6026l_nbytes_write(client, 0x6c, sy6026l_reg_drc_6c, 3);
	sy6026l_nbytes_write(client, 0x6d, sy6026l_reg_drc_6d, 3);
	sy6026l_nbytes_write(client, 0x6e, sy6026l_reg_drc_6e, 3);
}

// ***************************************************************************
// **** below are parameters for setting ****
// ***************************************************************************
// you may define these in sy6026l.h instead

//	eq parameters
const u8 sy6026l_reg_eq_30[] = {0x1f, 0x86, 0x62, 0x73, 0x00, 0xf7, 0x08, 0x43, 0x00, 0x78, 0xc9, 0xf4, 0x1f, 0x08, 0xf7, 0xbe, 0x00, 0x80, 0xd3, 0x9a};
const u8 sy6026l_reg_eq_31[] = {0x1f, 0x81, 0x66, 0xd6, 0x00, 0xfe, 0x66, 0xeb, 0x00, 0x7e, 0x4f, 0x27, 0x1f, 0x01, 0x99, 0x16, 0x00, 0x80, 0x4a, 0x04};
const u8 sy6026l_reg_eq_32[] = {0x1f, 0x80, 0xe8, 0x34, 0x00, 0xff, 0x09, 0x7a, 0x00, 0x7f, 0x39, 0xb5, 0x1f, 0x00, 0xf6, 0x87, 0x00, 0x7f, 0xde, 0x18};
const u8 sy6026l_reg_eq_33[] = {0x1f, 0x80, 0xf4, 0x31, 0x00, 0xff, 0x0a, 0xea, 0x00, 0x7f, 0x85, 0xae, 0x1f, 0x00, 0xf4, 0xa4, 0x00, 0x7f, 0x85, 0xae};
const u8 sy6026l_reg_eq_34[] = {0x1f, 0x80, 0xf1, 0xc8, 0x00, 0xff, 0x0d, 0x53, 0x00, 0x7f, 0x86, 0xe3, 0x1f, 0x00, 0xf2, 0x3b, 0x00, 0x7f, 0x86, 0xe3};
const u8 sy6026l_reg_eq_35[] = {0x1f, 0x83, 0x87, 0xe8, 0x00, 0xfb, 0xb0, 0xdc, 0x00, 0x7c, 0x03, 0x13, 0x1f, 0x04, 0x4f, 0x25, 0x00, 0x80, 0x75, 0x06};
const u8 sy6026l_reg_eq_37[] = {0x1f, 0x80, 0x6e, 0x18, 0x00, 0xff, 0x89, 0xd6, 0x00, 0x7f, 0x67, 0x11, 0x1f, 0x00, 0x76, 0x2a, 0x00, 0x80, 0x2a, 0xd7};
const u8 sy6026l_reg_eq_38[] = {0x1f, 0x92, 0xd2, 0x03, 0x00, 0xe6, 0x5c, 0x80, 0x00, 0x4c, 0x89, 0x0b, 0x1f, 0x5d, 0xe9, 0x8f, 0x00, 0x5c, 0x5e, 0xe4};
const u8 sy6026l_reg_eq_39[] = {0x1f, 0xd5, 0x57, 0x89, 0x1f, 0x87, 0x53, 0x97, 0x00, 0x48, 0xd5, 0x38, 0x00, 0x91, 0xaa, 0x70, 0x00, 0x48, 0xd5, 0x38};
const u8 sy6026l_reg_eq_3a[] = {0x1f, 0x80, 0xe4, 0x92, 0x00, 0xff, 0x16, 0xe7, 0x00, 0x7e, 0xfd, 0xd8, 0x1f, 0x00, 0xe9, 0x1a, 0x00, 0x80, 0x1d, 0x97};

//	drc parameters
const u8 sy6026l_reg_drc_47[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x7b, 0x11, 0xd3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x77, 0x17, 0x00, 0x02, 0x77, 0x17};
const u8 sy6026l_reg_drc_49[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x7b, 0x11, 0xd3, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x82, 0x77, 0x17, 0x00, 0x7d, 0x88, 0xe9};
const u8 sy6026l_reg_drc_4a[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x86, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x3c, 0x90, 0x00, 0x15, 0x3c, 0x90};
const u8 sy6026l_reg_drc_4c[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x86, 0xe1, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x95, 0x3c, 0x90, 0x00, 0x6a, 0xc3, 0x70};
const u8 sy6026l_reg_drc_4e[] = {0x01, 0x00, 0x00};
const u8 sy6026l_reg_drc_4f[] = {0x7f, 0xf8, 0x00};
const u8 sy6026l_reg_drc_50[] = {0x01, 0x00, 0x00};
const u8 sy6026l_reg_drc_51[] = {0x7f, 0xf0, 0x00};
const u8 sy6026l_reg_drc_52[] = {0x01, 0x00, 0x00};
const u8 sy6026l_reg_drc_53[] = {0x7f, 0x00, 0x00};
const u8 sy6026l_reg_drc_60[] = {0x01, 0x00, 0x00, 0x0f};
const u8 sy6026l_reg_drc_61[] = {0x3c, 0xc3, 0x04};
const u8 sy6026l_reg_drc_64[] = {0x3c, 0x53, 0x04};
const u8 sy6026l_reg_drc_67[] = {0x3c, 0xc3, 0x04};
const u8 sy6026l_reg_drc_6a[] = {0x00, 0x03, 0x04};
const u8 sy6026l_reg_drc_62[] = {0x7f, 0xbb, 0xcd};
const u8 sy6026l_reg_drc_65[] = {0x7f, 0xbb, 0xcd};
const u8 sy6026l_reg_drc_68[] = {0x7f, 0xbb, 0xcd};
const u8 sy6026l_reg_drc_6b[] = {0x7f, 0xbb, 0xcd};
const u8 sy6026l_reg_drc_63[] = {0x7f, 0xfc, 0x96};
const u8 sy6026l_reg_drc_66[] = {0x7f, 0xfc, 0x96};
const u8 sy6026l_reg_drc_69[] = {0x7f, 0xfc, 0x96};
const u8 sy6026l_reg_drc_6c[] = {0x7f, 0xfc, 0x96};
const u8 sy6026l_reg_drc_6d[] = {0x01, 0x00, 0x00};
const u8 sy6026l_reg_drc_6e[] = {0x7f, 0xf0, 0x00};
                           
// power stage setting
const u8 sy6026l_reg_outflip_55[] = {0x40, 0x20, 0x32, 0x10}; 	
const u8 sy6026l_reg_pwmctrl_56[] = {0x00, 0x00, 0x00, 0x2E}; 

// mixer setting
const u8 sy6026l_reg_mixer_5f[] = {0x1B, 0x1B, 0x1B, 0x1B};  // out_ch1 = out_ch2 = 0.5*in_ch1+0.5*in_ch2
                           
                           
                           
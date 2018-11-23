
// function declaration

static int sy6026l_probe(struct i2c_client *client,
			 const struct i2c_device_id *id);
			 
static int sy6026l_nbytes_write(struct i2c_client *client,
		unsigned int reg, const long values[], size_t len);
		
static int sy6026l_byte_write(struct i2c_client *client, u8 reg, u8 val);


void sy6026l_initial_simple_setting(struct i2c_client *client);
void sy6026l_initial_full_setting(struct i2c_client *client);
void sy6026l_pwmstage_write(struct i2c_client *client);
void sy6026l_faultfunction_write(struct i2c_client *client);
void sy6026l_mixer_write(struct i2c_client *client);
void sy6026l_eq_change(struct i2c_client *client);
void sy6026l_drc_change(struct i2c_client *client);
void sy6026l_before_eq_drc_write(struct i2c_client *client);
void sy6026l_after_eq_drc_write(struct i2c_client *client);
void sy6026l_eq_write(struct i2c_client *client);
void sy6026l_drc_write(struct i2c_client *client);

//basic registers

#define		PWMMODE_REG			0x22	// 1byte, exit standby and shutdown
#define		SYS_CTL_REG1		0x03	// 1byte,
#define		DSP_CTL_REG1		0x16	// channel enable

// volume registers
#define		MAS_VOL				0x07	// 1byte, master volume
#define		CH1_VOL				0x08	// 1byte, channel 1 volume
#define		CH2_VOL				0x09	// 1byte, channel 2 volume
#define		PRE_SCALER			0x36	// 4byties, prescaler volume before DSP
#define		POST_SCALER			0x0A	// 4byties, postscaler volume after DSP
#define		FINE_VOL			0x0B	// 1byte, linear volume

#define		I2S_CONTROL			0x15	// 1byte, I2S data type
#define		SOFT_MUTE 			0x06	// 1byte, soft mute

#define		HP_CTRL				0x1A	// 1byte, only for SY6035

// EQ enable
#define     CH1_EQ_CTRL1		0x25	// 1byte
#define     CH1_EQ_CTRL2		0x26	// 1byte
#define     CH2_EQ_CTRL1		0x27	// 1byte
#define     CH2_EQ_CTRL2		0x28	// 1byte
#define     SPEQ_CTRL_1			0x29	// 1byte
#define     SPEQ_CTRL_2 		0x2A	// 1byte
#define     SPEQ_CTRL_3			0x2B	// 1byte

// bq0~bq13 coeficients
#define     CH12_BQ0			0x30	// 20byties
#define     CH12_BQ1			0x31	// 20byties
#define     CH12_BQ2			0x32	// 20byties
#define     CH12_BQ3			0x33	// 20byties
#define     CH12_BQ4			0x34	// 20byties
#define     CH12_BQ5			0x35	// 20byties
#define     CH12_BQ6			0x37	// 20byties
#define     CH12_BQ7			0x38	// 20byties
#define     CH12_BQ8			0x39	// 20byties
#define     CH12_BQ9			0x3A	// 20byties
#define     CH12_BQ10			0x3B	// 20byties
#define     CH12_BQ11			0x3C	// 20byties
#define		CH12_BQ12			0x3D	// 20byties
#define		CH12_BQ13			0x3E	// 20byties
// SPEQ0~3 coeficients
#define		CH12_COEF_SPEQ0		0x43	// 12byties
#define		CH12_COEF_SPEQ1		0x44	// 12byties
#define		CH12_COEF_SPEQ2		0x45	// 12byties
#define		CH12_COEF_SPEQ3		0x46	// 12byties

// 20 byties registers for DRC biquad0~15 filter coeficients
#define		LDRC_BQ0			0x47	// 20byties
#define		LDRC_BQ1			0x48	// 20byties
#define		MDRC_BQ0			0x49	// 20byties
#define		MDRC_BQ1			0x4A	// 20byties
#define		HDRC_BQ0			0x4B	// 20byties
#define		HDRC_BQ1			0x4C	// 20byties

#define		MIXER_CTRL			0x5F	// 4byties

#define		LDRC_ENVLP_TC_UP	0x4E	// 4byties	
#define		LDRC_ENVLP_TC_DN	0x4F	// 4byties
#define		MDRC_ENVLP_TC_UP	0x50	// 4byties
#define		MDRC_ENVLP_TC_DN	0x51	// 4byties
#define		HDRC_ENVLP_TC_UP	0x52	// 4byties
#define		HDRC_ENVLP_TC_DN	0x53	// 4byties
#define		PDRC_ENVLP_TC_UP	0x6D	// 4byties
#define		PDRC_ENVLP_TC_DN	0x6E	// 4byties

#define		DRC_CTRL			0x60	// 4byties	
#define		LDRC_LMT_CFG1		0x61	// 3byties
#define		LDRC_LMT_CFG2		0x62	// 3byties
#define		LDRC_LMT_CFG3		0x63	// 3byties
#define		MDRC_LMT_CFG1		0x64	// 3byties
#define		MDRC_LMT_CFG2		0x65	// 3byties
#define		MDRC_LMT_CFG3		0x66	// 3byties
#define		HDRC_LMT_CFG1		0x67	// 3byties
#define		HDRC_LMT_CFG2		0x68	// 3byties
#define		HDRC_LMT_CFG3		0x69	// 3byties
#define		PDRC_LMT_CFG1		0x6A	// 3byties
#define		PDRC_LMT_CFG2		0x6B	// 3byties
#define		PDRC_LMT_CFG3		0x6C	// 3byties


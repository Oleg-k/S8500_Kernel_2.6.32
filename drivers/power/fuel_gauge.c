/* MAX17043 */
#define MAX17043

/* Slave address */
//#define MAX17043_SLAVE_ADDR	0x6D

/* Register address */
#define VCELL0_REG			0x02
#define VCELL1_REG			0x03
#define SOC0_REG			0x04
#define SOC1_REG			0x05
#define MODE0_REG			0x06
#define MODE1_REG			0x07
#define RCOMP0_REG			0x0C
#define RCOMP1_REG			0x0D
#define CMD0_REG			0xFE
#define CMD1_REG			0xFF

unsigned int FGPureSOC = 0;
unsigned int prevFGSOC = 0;
//unsigned int full_soc = 9300;
unsigned int fg_zero_count = 0;
//u8 preData[2]; /* for fg stress test */

int fuel_guage_init = 1;
EXPORT_SYMBOL(fuel_guage_init);


unsigned int fg_read_vcell(void)
{
	
//	u8 data[2];
	u32 vcell = 0;
        vcell = 3878;
	pr_info("%s: VCELL=%d\n", __func__, vcell);
	
	return vcell;
}

unsigned int fg_read_soc(void)
{
	u8 data[2];
	unsigned int FGAdjustSOC = 0;
	unsigned int FGSOC = 0;
	
	FGPureSOC = 7000;
	
	//if(FGPureSOC >= 180)
	if(FGPureSOC >= 0)
	{
		//FGAdjustSOC = ((FGPureSOC - 180)*10000)/9540;
		//FGAdjustSOC = (FGPureSOC*10000)/full_soc;
		FGAdjustSOC = (FGPureSOC*10000)/9300;
	}
	else
	{
		FGAdjustSOC = 0;
	}

	//calculating alert level
	// (2 - 1.8)/95.4*100 = 0.21%, so rounded off 0% so, 2% is ok! => apply to sbl source related rcomp
	
	// rounding off and Changing to percentage.
	FGSOC=FGAdjustSOC/100;

	if(FGAdjustSOC%100 >= 50 )
	{
		FGSOC+=1;
	}

	if(FGSOC>=100)
	{
		FGSOC=100;
	}

	/* we judge real 0% after 3 continuous counting */
	if(FGSOC == 0)
	{
		fg_zero_count++;

		if(fg_zero_count >= 3)
		{
			FGSOC = 0;
			fg_zero_count = 0;
		}
		else
		{
			FGSOC = prevFGSOC;
		}
	}
	else
	{
		fg_zero_count=0;
	}

	if(prevFGSOC != FGSOC) {
		printk("[SOC] C:%d, P:%d\n", prevFGSOC, FGSOC);
	}

	prevFGSOC = FGSOC;

	/* for fg stress test */
	/*
	if((preData[0] != data[0]) || (preData[1] != data[1]))
	{
		printk("soc = %d data[0] = 0x%x, data[1] = 0x%x\n", FGSOC, data[0],data[1]);
	}

	preData[0] = data[0];
	preData[1] = data[1];
	*/
	
	return FGSOC;
	
}


unsigned int fg_reset_soc(void)
{
	s32 ret = 1;
	return ret;
}


void fuel_gauge_rcomp(void)
{
	u8 rst_cmd[2];
	s32 ret = 1;
	return;
}

unsigned int fg_read_rcomp(void)
{
	u8 data[2];
	u16 rcomp = 0;
	rcomp = 5000;
	pr_info("%s: read rcomp = 0x%x\n", __func__, rcomp);
	
	return rcomp;
}



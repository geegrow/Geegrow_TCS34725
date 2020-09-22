/**********************    Register Names    **********************************/
#define     RN_COMMAND           /* Specifies register address */
#define     RN_ENABLE    0x00    /* Enables states and interrupts */
#define     RN_ATIME     0x01    /* RGBC time */
#define     RN_WTIME     0x03    /* Wait time */
#define     RN_AILTL     0x04    /* RGBC interrupt low threshold low byte */
#define     RN_AILTH     0x05    /* RGBC interrupt low threshold high byte */
#define     RN_AIHTL     0x06    /* RGBC interrupt high threshold low byte */
#define     RN_AIHTH     0x07    /* RGBC interrupt high threshold high byte */
#define     RN_PERS      0x0C    /* Interrupt persistence filter */
#define     RN_CONFIG    0x0D    /* Configuration */
#define     RN_CONTROL   0x0F    /* Control */
#define     RN_ID        0x12    /* Device ID */
#define     RN_STATUS    0x13    /* Device status */
#define     RN_CDATAL    0x14    /* Clear data low byte */
#define     RN_CDATAH    0x15    /* Clear data high byte */
#define     RN_RDATAL    0x16    /* Red data low byte */
#define     RN_RDATAH    0x17    /* Red data high byte */
#define     RN_GDATAL    0x18    /* Green data low byte */
#define     RN_GDATAH    0x19    /* Green data lhigh byte */
#define     RN_BDATAL    0x1A    /* Blue data low byte */
#define     RN_BDATAH    0x1B    /* Blue data high byte */

/*******************    Register descriptions    ******************************/
/* RN_COMMAND */
#define     RN_COMMAND_CMD               0x80  /* Select Command Register. Must write as 1 when addressing COMMAND register */
#define     RN_COMMAND_TYPE_REPEAT       0x00  /* Repeated byte protocol transaction */
#define     RN_COMMAND_TYPE_AUTOINC      0x20  /* Auto-increment protocol transaction */
#define     RN_COMMAND_TYPE_SF           0x60  /* Special function */
#define     RN_COMMAND_ADDRSF_CLR_IRQ    0x06  /* Clear channel interrupt clear */

/* RN_ENABLE */
#define     RN_ENABLE_AIEN    0x10    /* RGBC interrupt enable */
#define     RN_ENABLE_WEN     0x08    /* Wait enable */
#define     RN_ENABLE_AEN     0x02    /* RGBC enable */
#define     RN_ENABLE_PON     0x01    /* Power ON */

/* RN_ATIME */
#define     RN_ATIME_INTEG_TIME_2_4    0xFF    /* PRGBS integration time 2.4 ms (1 cycle) */
#define     RN_ATIME_INTEG_TIME_24     0xF6    /* PRGBS integration time 24 ms (10 cycles) */
#define     RN_ATIME_INTEG_TIME_101    0xD5    /* PRGBS integration time 101 ms (42 cycles) */
#define     RN_ATIME_INTEG_TIME_154    0xC0    /* PRGBS integration time 154 ms (64 cycles) */
#define     RN_ATIME_INTEG_TIME_700    0x00    /* PRGBS integration time 700 ms (256 cycles) */

/* RN_WTIME */
#define     RN_WTIME_2_4    0xFF    /* Wait time 2.4 ms (WLONG = 0) or 0.029 sec (WLONG = 1) */
#define     RN_WTIME_204    0xAB    /* Wait time 204 ms (WLONG = 0) or 2.45 sec (WLONG = 1) */
#define     RN_WTIME_614    0x00    /* Wait time 614 ms (WLONG = 0) or 7.4 sec (WLONG = 1) */

/* RN_PERS */
#define     RN_PERS_CONSEQ_VAL_0     0x00    /* Every RGBC cycle generates an interrupt */
#define     RN_PERS_CONSEQ_VAL_1     0x01    /* 1 value outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_2     0x02    /* 2 consequent values outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_3     0x03    /* 3 consequent values outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_5     0x04    /* 5 consequent values outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_10    0x05    /* 10 consequent values outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_15    0x06    /* 15 consequent values outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_20    0x07    /* 20 consequent values outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_25    0x08    /* 25 consequent values outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_30    0x09    /* 30 consequent values outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_35    0x0A    /* 35 consequent values outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_40    0x0B    /* 40 consequent values outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_45    0x0C    /* 45 consequent values outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_50    0x0D    /* 50 consequent values outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_55    0x0E    /* 55 consequent values outside of threshold range */
#define     RN_PERS_CONSEQ_VAL_60    0x0F    /* 60 consequent values outside of threshold range */

/* RN_CONFIG */
#define     RN_CONFIG_WLONG    0x02    /* Wait Long for the WTIME register */

/* RN_CONTROL */
#define     RN_CONTROL_GAIN_1X    0x00    /* RGBC gain value 1X gain */
#define     RN_CONTROL_GAIN_4X    0x01    /* RGBC gain value 4X gain */
#define     RN_CONTROL_GAIN_16X   0x02    /* RGBC gain value 16X gain */
#define     RN_CONTROL_GAIN_60X   0x03    /* RGBC gain value 60X gain */

/* RN_ID */
#define     RN_ID_ID    0x44    /* ID of TCS34725 device */

/* RN_STATUS */
#define     RN_STATUS_AINT    0x10    /* RGBC clear channel Interrupt */
#define     RN_STATUS_AVALID  0x01    /* RGBC Valid. Indicates that the RGBC channels have completed an integration cycle */
#define TAG "RDA"

#include <stdio.h>         //鏍囧噯杈撳叆杈撳嚭瀹氫箟
#include <stdlib.h>        //鏍囧噯鍑芥暟搴撳畾涔�
#include <unistd.h>       //Unix鏍囧噯鍑芥暟瀹氫箟
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>          //鏂囦欢鎺у埗瀹氫箟
#include <termios.h>     //POSIX涓柇鎺у埗瀹氫箟
#include <errno.h>        //閿欒鍙峰畾涔�
#include "rda5876.h"
#include "zl/log.h"

typedef unsigned int __u32;
typedef unsigned char __u8;
typedef unsigned short __u16;
typedef unsigned char T_U8;
typedef unsigned long int T_U32;
typedef unsigned long int uint24_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

typedef struct
{
    uint24_t lap;		/* Lower Address Part 00..23 */
    uint8_t  uap;               /* upper Address Part 24..31 */
    uint16_t nap;		/* Non-significant    32..47 */
} BD_ADDR_T;

extern void Pwd_mcu();
extern int Pwd_flag();
extern unsigned char *GetMcuLocalBtAddr();
unsigned char *GetSerialIndex();
BD_ADDR_T *GetLocalAddr();
static  int serial_fd = -1;
BD_ADDR_T *g_LocalBtAddr = NULL;
char g_pComPort[128] = {0};

unsigned char *GetSerialIndex(){
	return g_pComPort;
}

BD_ADDR_T *GetLocalAddr(){
	return g_LocalBtAddr;
}
static void uart_write_data(char *data, unsigned short data_len) //send data to UART
{
	int nwrite;
	if ((nwrite = write(serial_fd, data, data_len)) != data_len) {
		printf("towrite : %d,written:%d\n", data_len, nwrite);
	}
	//printf("uart write data %d\n", data_len);
}

static void rdabt_write_memory(__u32 addr,const __u32 *data, __u8 len, __u8 memory_type) {
	__u16 num_to_send;
	__u16 i, j;
	__u8 data_to_send[256] = { 0 };
	__u32 address_convert;
	data_to_send[0] = 0x01;
	data_to_send[1] = 0x02;
	data_to_send[2] = 0xfd;
	data_to_send[3] = (__u8 ) (len * 4 + 6);
	data_to_send[4] = (memory_type) + 0x80; // add the event display
	data_to_send[5] = len;
	if (memory_type == 0x01) {
		address_convert = addr * 4 + 0x200;
		data_to_send[6] = (__u8 ) address_convert;
		data_to_send[7] = (__u8 ) (address_convert >> 8);
		data_to_send[8] = (__u8 ) (address_convert >> 16);
		data_to_send[9] = (__u8 ) (address_convert >> 24);
	} else {
		data_to_send[6] = (__u8 ) addr;
		data_to_send[7] = (__u8 ) (addr >> 8);
		data_to_send[8] = (__u8 ) (addr >> 16);
		data_to_send[9] = (__u8 ) (addr >> 24);
	}
	for (i = 0; i < len; i++, data++) {
		j = 10 + i * 4;
		data_to_send[j] = (__u8 ) (*data);
		data_to_send[j + 1] = (__u8 ) ((*data) >> 8);
		data_to_send[j + 2] = (__u8 ) ((*data) >> 16);
		data_to_send[j + 3] = (__u8 ) ((*data) >> 24);
	}
	num_to_send = 4 + data_to_send[3];

	uart_write_data(&(data_to_send[0]), num_to_send);	//guochao
}

static void RDA_uart_write_array(const __u32 buf[][2], __u16 len, __u8 type) {
	__u32 i;
	for (i = 0; i < len; i++) {
		rdabt_write_memory(buf[i][0], &buf[i][1], 1, type);
		usleep(12000);	//12ms?
	}
}

static const __u32 rdabt_rf_init_12[][2] = {
	{ 0x3f, 0x0000 }, //;page 0
	{ 0x01, 0x1FFF }, //;
	{ 0x06, 0x07F7 }, //;padrv_set,increase the power.
	{ 0x08, 0x01E7 }, //;
	{ 0x09, 0x0520 }, //;
	{ 0x0B, 0x03DF }, //;filter_cap_tuning<3:0>1101
	{ 0x0C, 0x85E8 }, //;
	{ 0x0F, 0x0DBC }, //; 0FH,16'h1D8C; 0FH,16'h1DBC;adc_clk_sel=1 20110314 ;adc_digi_pwr_reg<2:0>=011;
	{ 0x12, 0x07F7 }, //;padrv_set,increase the power.
	{ 0x13, 0x0327 }, //;agpio down pullen .
	{ 0x14, 0x0CCC }, //;h0CFE; bbdac_cm 00=vdd/2.
	{ 0x15, 0x0526 }, //;Pll_bypass_ontch:1,improve ACPR.
	{ 0x16, 0x8918 }, //;add div24 20101126
	{ 0x18, 0x8800 }, //;add div24 20101231
	{ 0x19, 0x10C8 }, //;pll_adcclk_en=1 20101126
	{ 0x1A, 0x9128 }, //;Mdll_adcclk_out_en=0
	{ 0x1B, 0x80C0 }, //;1BH,16'h80C2
	{ 0x1C, 0x361f }, //;
	{ 0x1D, 0x33fb }, //;Pll_cp_bit_tx<3:0>1110;13D3
	{ 0x1E, 0x303f }, //;Pll_lpf_gain_tx<1:0> 00;304C
	//{0x23,0x3335}, //;txgain
	//{0x24,0x8AAF}, //;
	//{0x27,0x1112}, //;
	//{0x28,0x345F}, //;
	{ 0x23, 0x2222 }, //;txgain
	{ 0x24, 0x359d }, //;
	{ 0x27, 0x0011 }, //;
	{ 0x28, 0x124f }, //;

	{ 0x39, 0xA5FC }, //;
	{ 0x3f, 0x0001 }, //;page 1
	{ 0x00, 0x043F }, //;agc
	{ 0x01, 0x467F }, //;agc
	{ 0x02, 0x28FF }, //;agc//20110323;82H,16'h68FF;agc
	{ 0x03, 0x67FF }, //;agc
	{ 0x04, 0x57FF }, //;agc
	{ 0x05, 0x7BFF }, //;agc
	{ 0x06, 0x3FFF }, //;agc
	{ 0x07, 0x7FFF }, //;agc
	{ 0x18, 0xf3f5 }, //;
	{ 0x19, 0xf3f5 }, //;
	{ 0x1A, 0xe7f3 }, //;
	{ 0x1B, 0xf1ff }, //;
	{ 0x1C, 0xffff }, //;
	{ 0x1D, 0xffff }, //;
	{ 0x1E, 0xffff }, //;
	{ 0x1F, 0xFFFF }, //;padrv_gain;9FH,16'hFFEC;padrv_gain20101103

	{ 0x23, 0x4224 }, //;ext32k   //  //lugong modify,if mark it ,you can repeat open/close bt without 32k.

	{ 0x24, 0x0110 }, { 0x25, 0x43E1 }, //;ldo_vbit:110,2.04v
	{ 0x26, 0x4bb5 }, //;reg_ibit:100,reg_vbit:101,1.2v,reg_vbit_deepsleep:110,750mV
	{ 0x32, 0x0079 }, //;TM mod
	{ 0x3f, 0x0000 }, //;page 0
};

static const __u32 RDA5876_ENABLE_SPI[][2] = { { 0x40240000, 0x2004f39c }, };

static const __u32 RDA5876_DISABLE_SPI[][2] = { { 0x40240000, 0x2000f29c }, };

static const __u32 RDA5876_PSKEY_RF[][2] = {
	{ 0x40240000, 0x2004f39c }, //; SPI2_CLK_EN PCLK_SPI2_EN
	{ 0x800000C0, 0x00000021 }, //; CHIP_PS PSKEY: Total number -----------------
	{ 0x800000C4, 0x003F0000 },
	{ 0x800000C8, 0x00414003 },
	{ 0x800000CC, 0x004225BD },
	{ 0x800000D0, 0x004908E4 },
	{ 0x800000D4, 0x0043B074 },
	{ 0x800000D8, 0x0044D01A },
	//{0x800000DC,0x004A0600},
	{ 0x800000DC, 0x004A0800 },
	{ 0x800000E0, 0x0054A020 },
	{ 0x800000E4, 0x0055A020 },
	{ 0x800000E8, 0x0056A542 },
	{ 0x800000EC,0x00574C18 },
	{ 0x800000F0, 0x003F0001 },
	{ 0x800000F4, 0x00410900 },
	{ 0x800000F8, 0x0046033F },
	{ 0x800000FC, 0x004C0000 },
	{ 0x80000100, 0x004D0015 },
	{ 0x80000104, 0x004E002B },
	{ 0x80000108, 0x004F0042 },
	{ 0x8000010C, 0x0050005A },
	{ 0x80000110, 0x00510073 },
	{ 0x80000114, 0x0052008D },
	{ 0x80000118, 0x005300A7 },
	{ 0x8000011C, 0x005400C4 },
	{ 0x80000120, 0x005500E3 },
	{ 0x80000124, 0x00560103 },
	{ 0x80000128, 0x00570127 },
	{ 0x8000012C, 0x0058014E },
	{ 0x80000130, 0x00590178 },
	{ 0x80000134, 0x005A01A1 },
	{ 0x80000138, 0x005B01CE },
	{ 0x8000013C, 0x005C01FF },
	{ 0x80000140, 0x003F0000 },
	{ 0x80000144, 0x00000000 }, //; PSKEY: Page 0
	{ 0x80000040, 0x10000000 },
    //adde for mini sleep (quick switch sco will failed)
	//{0x80000078,0x0f054001},
	//{0x80000040,0x00004000},

	//{0x40240000,0x2000f29c},
	//{0x40240000,0x0000f29c}, //; SPI2_CLK_EN PCLK_SPI2_EN
};

static const __u32 RDA5876_DCCAL[][2] = { { 0x00000030, 0x00000129 }, { 0x00000030,
		0x0000012b } };

//static const __u32 RDA5876_PSKEY_MISC[][2] = {
static __u32 RDA5876_PSKEY_MISC[][2] = {
//	{0x80000ab0,0x80600000},
	{ 0x800004ec, 0xf88dfe7f },    //shut down edr
	{ 0x800004f0, 0x83711f98 }, ///disable 3m esco ev4 ev5
	{ 0x80002768, 0x00040006 }, //BTq_l2cap_in.num_of_free_headers=4  BTq_l2cap_in.num_of_headers=6
	{ 0x800006f8, 0x00b400dc }, //hc_flow_ctrl.host_ACL_Data_Packet_Length=0xdc,host_SYN_Data_Packet_Length=0xb4
	{ 0x80000044, 0xaabbccdd },  //33445566
	{ 0x80000048, 0xabacadae },  //00001122
	{ 0x8000005c, 0x036e0201 },  //No parity -- 20140320 lin
	//{ 0x8000005c, 0x1b6e0201 }, //Even Parity uart_settings=0x1b convert_time=0x6e<<3 uart_flags=0x02 type=0x01
	{ 0x80000060,     500000 }, ///baudrate 0x000e1000 //0x0001c200 //0x00054600

	//{ 0x80000070, 0x00014088 }, //b4 is set this, we need change?
	//0x08  DISABLE_ESCO_LINK
	//0x80  FORCE_SCO_VIA_HCI
	//0x200 DISABLE_FIX_SCO_CFG
//	{0x80000070, 0x00014280},
	{0x80000070, 0x00004086},

	{ 0x80000074, 0xA5025010 },
	{ 0x8000007c, 0xb530b530 },

	{ 0x80000084, 0x808d0007 },	//pcm config
//	{ 0x80000084, 0x808d000E },	//pcm config

	{ 0x80000098, 0x9a610020 },	//packet_count=0x20 bcsp_flags=0x00 bcsp_resend_wake=0x61( g_bcsp_resend_time_slot=0xc0) g_bcsp_send_win_size=0x82(g_bcsp_flags=0x80,winsize=2)
	{ 0x800000a0, 0x00000000 },	//g_host_wake_event1 g_host_wake_event2=0
	{ 0x800000a4, 0x00000000 },	//g_host_wake_flag = 0xf2 g_host_wake_length = 0x0a g_host_wake_pause_length = 0x0a
	{ 0x800000a8, 0x0Bbaba30 },	//min power level
	{ 0x80000040, 0x0742b186 },
};

static const __u32 RDA5876_TRAP[][2] = {
	{ 0x80000098, 0x9a610020 },
	{ 0x80000040, 0x00400000 },
	{ 0x40180100, 0x000068b0 }, //inc power
	{ 0x40180120, 0x000068f4 }, { 0x40180104, 0x000066b0 }, //dec power
	{ 0x40180124, 0x000068f4 }, { 0x40180108, 0x00017354 }, // auth after encry
	{ 0x40180128, 0x00017368 }, { 0x4018010c, 0x0001a338 }, // auth after encry
	{ 0x4018012c, 0x00000014 }, { 0x40180110, 0x0001936c }, // pair after auth fail
	{ 0x40180130, 0x00001ca8 }, { 0x40180114, 0x0000f8c4 }, ///all rxon
	{ 0x40180134, 0x00026948 }, { 0x40180118, 0x000130b8 }, ///qos PRH_CHN_QUALITY_MIN_NUM_PACKETS
	{ 0x40180138, 0x0001cbb4 }, { 0x4018011c, 0x00010318 }, //lu added for earphone 09.27
	{ 0x4018013c, 0x00001f04 },

	{ 0x40180020, 0x00015a14 }, // Set Wesco to 2
	{ 0x40180040, 0x000174b4 },

	{ 0x8000000c, 0xea00003e },
	{ 0x80000104, 0xe51ff004 },
	{ 0x80000108, 0x00001ec0 },
	{ 0x8000010c, 0xe3a00000 },
	{ 0x80000110, 0xe3a01000 },
	{ 0x80000114, 0xebfffffa },
	{ 0x80000118, 0xe3500000 },
	{ 0x8000011c, 0x03a0ebcb },
	{ 0x80000120, 0x028ef020 },
	{ 0x80000124, 0xe3a02004 },
	{ 0x80000128, 0xe3a0ebca },
	{ 0x8000012c, 0xe28effe9 },
	{ 0x4018001c, 0x00032ba0 },
	{ 0x4018003c, 0x00032d20 },

	{ 0x80000014, 0xea000055 },
	{ 0x80000170, 0xe59f0024 },
	{ 0x80000174, 0xe5801000 },
	{ 0x80000178, 0xe3a00002 },
	{ 0x8000017c, 0xe59f100c },
	{ 0x80000180, 0xe1a02004 },
	{ 0x80000184, 0xe3a03001 },
	{ 0x80000188, 0xe59fe004 },
	{ 0x8000018c, 0xe59ff004 },
	{ 0x80000190, 0x800001a0 },
	{ 0x80000194, 0x0000b6ec },
	{ 0x80000198, 0x0001bab8 },
	{ 0x8000019c, 0x800001b4 },
	{ 0x800001a0, 0xe59f1004 },
	{ 0x800001a4, 0xe5911000 },
	{ 0x800001a8, 0xe59ff008 },
	{ 0x800001ac, 0x800001b4 },
	{ 0x800001b0, 0x00030648 },
	{ 0x800001b4, 0x00000000 },
	{ 0x800001b8, 0x00030648 },
	{ 0x40180018, 0x0000b6e8 },
	{ 0x40180038, 0x00032d28 },

	{ 0x80000018, 0xea000078 },    //   sniff in sco
	{ 0x80000200, 0x05d41027 },
	{ 0x80000204, 0x00611181 },
	{ 0x80000208, 0x00811004 },
	{ 0x8000020c, 0x05d1102a },
	{ 0x80000210, 0x01510000 },
	{ 0x80000214, 0xe3a00bbb },
	{ 0x80000218, 0x1280ff79 },
	{ 0x8000021c, 0x0280ff59 },
	{ 0x40180014, 0x0002ed60 },
	{ 0x40180034, 0x00032d2c },

	{ 0x40180004, 0x0002192c }, //
	{ 0x40180024, 0x00006b38 }, //
//	{0x40180000,0x0000fff1},

	//repair ZTEU807 no audio question -- 20140210
	{0x40180008,0x00016ac0}, // esco air mode
	{0x40180028,0x0001fbf4},
	{0x40180000,0x0000fff3},

	/*{0x4018000C,0x00024020},
	{0x4018002C,0x00000014},
	{0x40180000,0x0000fff7},*/

	{ 0x8000054c, 0x007803fd }, // set buffer size
	{ 0x80000550, 0x00030004 },
	{ 0x80000554, 0x007803fd },
	{ 0x80000558, 0x00030002 },
	{ 0x8000055c, 0x00010078 },
	{ 0x80000560, 0x00000003 },

};

static void RDA5876_RfInit() {
	RDA_uart_write_array(RDA5876_ENABLE_SPI, sizeof(RDA5876_ENABLE_SPI) / sizeof(RDA5876_ENABLE_SPI[0]), 0);
	RDA_uart_write_array(rdabt_rf_init_12, sizeof(rdabt_rf_init_12) / sizeof(rdabt_rf_init_12[0]), 1);
	usleep(50000); //50ms?
}

static void RDA5876_Pskey_RfInit() {
	RDA_uart_write_array(RDA5876_PSKEY_RF, sizeof(RDA5876_PSKEY_RF) / sizeof(RDA5876_PSKEY_RF[0]), 0);
}

static void RDA5876_Dccal() {
	RDA_uart_write_array(RDA5876_DCCAL, sizeof(RDA5876_DCCAL) / sizeof(RDA5876_DCCAL[0]), 1);
	RDA_uart_write_array(RDA5876_DISABLE_SPI, sizeof(RDA5876_DISABLE_SPI) / sizeof(RDA5876_DISABLE_SPI[0]), 0);
}

static void RDA5876_Pskey_Misc() {
	RDA_uart_write_array((const __u32 (*)[2])RDA5876_PSKEY_MISC, sizeof(RDA5876_PSKEY_MISC) / sizeof(RDA5876_PSKEY_MISC[0]), 0);
}

static void RDA5876_Trap() {
	RDA_uart_write_array(RDA5876_TRAP, sizeof(RDA5876_TRAP) / sizeof(RDA5876_TRAP[0]), 0);
}

static void RDA_pin_to_high(void) {
}

static void RDA_pin_to_low(void) {
}

int rdabt_poweroff(void) {
	RDA_pin_to_low();
	return 1;
}

static int uart_open() {
	int fd = open(g_pComPort, O_RDWR | O_NOCTTY);
	if (fd == -1) {
		perror("涓插彛1鎵撳紑澶辫触 ");
		return -1;
	}
	struct termios opt;    //瀹氫箟termios缁撴瀯

	if (tcgetattr(fd, &opt) != 0) {
		printf("tcgetattr fd");
		return -1;
	}

	cfsetispeed(&opt, B115200);
	cfsetospeed(&opt, B115200);

	opt.c_cflag &= ~CSIZE;
	opt.c_cflag |= CS8;

	opt.c_cflag &= ~PARENB;        //娓呴櫎鏍￠獙浣�
	opt.c_iflag &= ~INPCK;        //enable parity checking

	opt.c_cflag &= ~CSTOPB;
	opt.c_cflag |= (CLOCAL | CREAD);

	opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	opt.c_oflag &= ~OPOST;
	opt.c_oflag &= ~(ONLCR | OCRNL);    //娣诲姞鐨�

	opt.c_iflag &= ~(ICRNL | INLCR);
	opt.c_iflag &= ~(IXON | IXOFF | IXANY);    //娣诲姞鐨�

	tcflush(fd, TCIFLUSH);
	opt.c_cc[VTIME] = 0;        //璁剧疆瓒呮椂涓�5sec
	opt.c_cc[VMIN] = 1;        //Update the opt and do it now

	if (tcsetattr(fd, TCSANOW, &opt) != 0) {
		perror("tcsetattr fd");
		return -1;
	}
	tcflush(fd, TCIFLUSH);
	return fd;
}

int rdabt_poweron_init(char *ComPort) {
	memcpy(g_pComPort, ComPort, strlen(ComPort));
	Pwd_mcu();
	if (Pwd_flag() == 0)
    	return -1;

	if ((serial_fd = uart_open()) == -1)
		return -1;

	unsigned char *getBtLocalAddr = GetMcuLocalBtAddr();
	__u32 addr[8] = {0};
	for(int i = 0; i < 8; i++){
		addr[i] = getBtLocalAddr[i];
	}

	uint16_t  value[4];
	value[0] = (addr[0] << 8) + addr[1];
	value[1] = (addr[2] << 8) + addr[3];
	value[2] = (addr[4] << 8) + addr[5];
	value[3] = (addr[6] << 8) + addr[7];

	//LOGD("BtAddr [%04x %04x %04x %04x]\n",
	//		value[0], value[1], value[2], value[3]);

	BD_ADDR_T Bt_addr;
	Bt_addr.lap = value[0];
	Bt_addr.lap <<= 16;
	Bt_addr.lap |= value[1];
	Bt_addr.uap = (value[2] >> 8) & 0x00FF;
	Bt_addr.nap = (value[3] >> 8) | (value[3] << 8);

	if(NULL == g_LocalBtAddr){
		g_LocalBtAddr = malloc(sizeof(BD_ADDR_T));
		memcpy(g_LocalBtAddr, &Bt_addr, sizeof(BD_ADDR_T));
	}

	addr[0] = (Bt_addr.nap & 0xFF00) >> 8;
	addr[1] = Bt_addr.nap & 0x00FF;
	addr[2] = Bt_addr.uap;
	addr[3] = Bt_addr.lap >> 16;
	addr[4] = (Bt_addr.lap & 0x00FF00) >> 8;
	addr[5] = Bt_addr.lap & 0x0000FF;

	RDA5876_PSKEY_MISC[5][1] = (addr[0] << 8) + addr[1];
	RDA5876_PSKEY_MISC[4][1] = (addr[2] << 24) + (addr[3] << 16) + (addr[4] << 8) + addr[5];

	LOGD("BtAddr [%02x %02x %02x %02x %02x %02x]\n",
			addr[0], addr[1], addr[2], addr[3],
			addr[4], addr[5]);

	//LOGD("RDA5876_PSKEY_MISC");
	//for(int i = 0; i < (sizeof(RDA5876_PSKEY_MISC) / sizeof(RDA5876_PSKEY_MISC[0])); i++){
	//	LOGD("{0x%08x, 0x%08x}", RDA5876_PSKEY_MISC[i][0], RDA5876_PSKEY_MISC[i][1]);
	//}

	RDA_pin_to_high();
	usleep(50000);

	RDA5876_RfInit();
	usleep(50000);

	RDA5876_Pskey_RfInit();
	usleep(50000);

	//	char buf[] = {0x01,0x1a,0xfc,0x06,0x00,0x0D,0x1A,0x7F,0xDD,0xBB};
	//	write(serial_fd,buf,10);
	//	sleep(2);

	RDA_pin_to_high();
	usleep(250000);

	RDA5876_RfInit();
	usleep(50000);

	RDA5876_Pskey_RfInit();
	usleep(50000);

	RDA5876_Dccal();
	usleep(50000);


	RDA5876_Trap();
	usleep(50000);

	RDA5876_Pskey_Misc();
	usleep(250000);

//	char buffer[4096];
//	int n = read(serial_fd, buffer, 4096);
//	printf("read %d \n", n);
//	for (int i = 0; i < n; i++) {
//		printf("%02X ", buffer[i]);
//	}
//	printf("\n");

	close(serial_fd);
	serial_fd = -1;

	usleep(1000);
	//printf("rdabt_poweron_init Leave\n");

	return 0;
}

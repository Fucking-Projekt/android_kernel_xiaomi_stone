#ifndef HQSYS_PCBA
#define HQSYS_PCBA

/*M17-T code for HQ-222139 by qianxiaoming at 2022/11/17 start*/
typedef enum {
   PCBA_UNKNOW = -1,

   PCBA_M17_P0_1_CN = 0x10,
   PCBA_M17_P0_1_GL,
   PCBA_M17_P0_1_IN,
   PCBA_M17_P0_1_CN_NEW,
   PCBA_M17P_P0_1_IN,
   PCBA_M17P_P0_1_GL,
   PCBA_M17X_P0_1_CN,

   PCBA_M17_P1_CN = 0x20,
   PCBA_M17_P1_GL,
   PCBA_M17_P1_IN,
   PCBA_M17_P1_CN_NEW,
   PCBA_M17P_P1_IN,
   PCBA_M17P_P1_GL,
   PCBA_M17X_P1_CN,

   PCBA_M17_P1_1_CN = 0x30,
   PCBA_M17_P1_1_GL,
   PCBA_M17_P1_1_IN,
   PCBA_M17_P1_1_CN_NEW,
   PCBA_M17P_P1_1_IN,
   PCBA_M17P_P1_1_GL,
   PCBA_M17X_P1_1_CN,

   PCBA_M17_P2_CN = 0x40,
   PCBA_M17_P2_GL,
   PCBA_M17_P2_IN,
   PCBA_M17_P2_CN_NEW,
   PCBA_M17P_P2_IN,
   PCBA_M17P_P2_GL,
   PCBA_M17X_P2_CN,

   PCBA_M17_MP_CN = 0x50,
   PCBA_M17_MP_GL,
   PCBA_M17_MP_IN,
   PCBA_M17_MP_CN_NEW,
   PCBA_M17P_MP_IN,
   PCBA_M17P_MP_GL,
   PCBA_M17X_MP_CN,
   PCBA_M17_MP_CN_ZS,

   PCBA_END,
} PCBA_CONFIG;
/*M17-T code for HQ-222139 by qianxiaoming at 2022/11/17 end*/
struct pcba_info {
	PCBA_CONFIG pcba_config;
	char pcba_name[32];
};

PCBA_CONFIG get_huaqin_pcba_config(void);

#endif

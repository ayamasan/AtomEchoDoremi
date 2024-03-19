#include <M5Atom.h>
#include <driver/i2s.h>

#include "s0.h"
#include "s1.h"
#include "s2.h"
#include "s3.h"
#include "s4.h"
#include "s5.h"
#include "s6.h"
#include "s7.h"
#include "s8.h"
#include "s9.h"
#include "s10.h"
#include "s11.h"
#include "s12.h"
#include "s13.h"
#include "s14.h"
#include "s15.h"
#include "s16.h"
#include "s17.h"
#include "s18.h"
#include "s19.h"
#include "s20.h"
#include "s21.h"
#include "s22.h"
#include "s23.h"
#include "s24.h"

#define CONFIG_I2S_BCK_PIN      19
#define CONFIG_I2S_LRCK_PIN     33
#define CONFIG_I2S_DATA_PIN     22
#define CONFIG_I2S_DATA_IN_PIN  23

#define SPEAKER_I2S_NUMBER      I2S_NUM_0

#define MODE_MIC                0
#define MODE_SPK                1

#define SNDLEN 16000

int sound = 0; // 1で演奏
int tempo = 120; // テンポ（1分あたりの拍数）
int tt = 60000 / tempo; // 1拍の時間（ミリ秒、テンポ120で500ミリ秒）
int rate = 16000; // サンプリングレート（160000がディフォルト）
unsigned char SONG[32000];  // 1秒分のバッファ


void InitI2SSpeakerOrMic(int mode)
{
	esp_err_t err = ESP_OK;
	
	i2s_driver_uninstall(SPEAKER_I2S_NUMBER);
	i2s_config_t i2s_config = {
		.mode                 = (i2s_mode_t)(I2S_MODE_MASTER),
		.sample_rate          = rate,
		.bits_per_sample      = I2S_BITS_PER_SAMPLE_16BIT,
		.channel_format       = I2S_CHANNEL_FMT_ALL_RIGHT,
		.communication_format = I2S_COMM_FORMAT_I2S,
		.intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1,
		.dma_buf_count        = 6,
		.dma_buf_len          = 60,
		.use_apll             = false,
		.tx_desc_auto_clear   = true,
		.fixed_mclk           = 0
	};
	
	if(mode == MODE_MIC){
		i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
	}
	else{
		i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
	}
	
	err += i2s_driver_install(SPEAKER_I2S_NUMBER, &i2s_config, 0, NULL);
	
	i2s_pin_config_t tx_pin_config = {
		.bck_io_num           = CONFIG_I2S_BCK_PIN,
		.ws_io_num            = CONFIG_I2S_LRCK_PIN,
		.data_out_num         = CONFIG_I2S_DATA_PIN,
		.data_in_num          = CONFIG_I2S_DATA_IN_PIN,
	};
	
	err += i2s_set_pin(SPEAKER_I2S_NUMBER, &tx_pin_config);
	
	if(mode != MODE_MIC){
		err += i2s_set_clk(SPEAKER_I2S_NUMBER, rate, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
	}
	
	i2s_zero_dma_buffer(SPEAKER_I2S_NUMBER);
}

void setsong(int num)
{
	int i;
	switch(num){
		case  0 : memcpy(SONG, s0, SNDLEN); break;
		case  1 : memcpy(SONG, s1, SNDLEN); break;
		case  2 : memcpy(SONG, s2, SNDLEN); break;
		case  3 : memcpy(SONG, s3, SNDLEN); break;
		case  4 : memcpy(SONG, s4, SNDLEN); break;
		case  5 : memcpy(SONG, s5, SNDLEN); break;
		case  6 : memcpy(SONG, s6, SNDLEN); break;
		case  7 : memcpy(SONG, s7, SNDLEN); break;
		case  8 : memcpy(SONG, s8, SNDLEN); break;
		case  9 : memcpy(SONG, s9, SNDLEN); break;
		case 10 : memcpy(SONG, s10, SNDLEN); break;
		case 11 : memcpy(SONG, s11, SNDLEN); break;
		case 12 : memcpy(SONG, s12, SNDLEN); break;
		case 13 : memcpy(SONG, s13, SNDLEN); break;
		case 14 : memcpy(SONG, s14, SNDLEN); break;
		case 15 : memcpy(SONG, s15, SNDLEN); break;
		case 16 : memcpy(SONG, s16, SNDLEN); break;
		case 17 : memcpy(SONG, s17, SNDLEN); break;
		case 18 : memcpy(SONG, s18, SNDLEN); break;
		case 19 : memcpy(SONG, s19, SNDLEN); break;
		case 20 : memcpy(SONG, s20, SNDLEN); break;
		case 21 : memcpy(SONG, s21, SNDLEN); break;
		case 22 : memcpy(SONG, s22, SNDLEN); break;
		case 23 : memcpy(SONG, s23, SNDLEN); break;
		case 24 : memcpy(SONG, s24, SNDLEN); break;
		default : 
			for(i=0; i<SNDLEN; i++){
				SONG[i] = 0;
			}
			break;
	}
}


void settempo(int t)
{
	tempo = t;
	tt = 60000 / tempo;
	//rate = (16000 * t) / 120;
	//InitI2SSpeakerOrMic(MODE_MIC);
}


void setup() 
{
	// put your setup code here, to run once:
	M5.begin(true, false, true);
	
	for(int i=0; i<5; i++){
		M5.dis.drawpix(0, CRGB(0, 0, 0));
		delay(200);
		M5.dis.drawpix(0, CRGB(255, 0, 0));
		delay(200);
	}
	// sin波作成（1sec分）
	for(int i=0; i<SNDLEN; i++){
		SONG[i] = s0[i];
	}
	sound = 0;
	tempo = 120;
	settempo(120);
	setsong(-1);
	
	M5.dis.drawpix(0, CRGB(0, 0, 255));
	InitI2SSpeakerOrMic(MODE_MIC);
	delay(2000);
}


void loop() 
{
	// put your main code here, to run repeatedly:
	int i, j;
	size_t bytes_written;
	M5.update();
	
	// ボタン押し
	if(M5.Btn.wasPressed()){
		M5.dis.drawpix(0, CRGB(255, 0, 0));
		InitI2SSpeakerOrMic(MODE_SPK);
		sound = 1;
	}
	#if 0
	// ボタン放し
	if(M5.Btn.wasReleased()){
		sound = 0;
		// Set Mic Mode
		InitI2SSpeakerOrMic(MODE_MIC);
		M5.dis.drawpix(0, CRGB(0, 0, 255));
	}
	#endif
	
	// 演奏
	if(sound){
		setsong(sound-1);
		// Write Speaker（データ転送完了まで待たされる）
		i2s_write(SPEAKER_I2S_NUMBER, SONG, SNDLEN, &bytes_written, portMAX_DELAY);
		i2s_zero_dma_buffer(SPEAKER_I2S_NUMBER);
		sound++;
		if(sound > 25){
			sound = 0;  // 終了
			// Set Mic Mode
			InitI2SSpeakerOrMic(MODE_MIC);
			M5.dis.drawpix(0, CRGB(0, 0, 255));
		}
	}
	else{
		delay(tt);
	}
}

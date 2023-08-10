/*************************************************
File:       		BMV56T123.h
Author:            	Tree, BESTMODULE
Description:        BMV56T123 function
History：			
V1.0	 -- initial version；2023-01-05；Arduino IDE : ≥v1.8.13

**************************************************/
#ifndef _BMV56T123_H_
#define _BMV56T123_H_
/*****************class for BM_musictouch*******************/
#include <SoftwareSerial.h>
#include <Arduino.h>

#define _CMD_SET_PLAY_MODE	(0x01)
#define _CMD_SET_VOICE	(0x02)
#define _CMD_SET_VOLUME	(0x03)
#define _CMD_PLAY_VOICE	(0x04)
#define _CMD_OPEN_LED	(0x05)
#define _CMD_CLOSE_LED	(0x06)
#define _CMD_GET_TOUCH_VALUE	(0x07)
#define INTERNAL_SOUND_SOURCE (0x00)
#define EXTERNAL_SOUND_SOURCE (0x01)
#define BAUDRATE	(115200)

class BMV56T123
{
	public:
	    BMV56T123(uint8_t intPin = 4, uint8_t rxPin = 2, uint8_t txPin = 3);
	    void begin(void);
        bool setPlayMode(uint8_t mode);
	    bool setTimbreGroup(uint8_t group);
	    bool setVolume(uint8_t volume);
	    bool playVoice(uint8_t voice_number);
		bool openLED(uint8_t led_number);
		bool closeLED(uint8_t led_number);
		uint16_t getTouchValue(void);
		uint8_t getINT(void);
    private:

		SoftwareSerial *_softSerial = NULL;	
		
        uint8_t _intPin;
        uint8_t _rxPin;
        uint8_t _txPin;		
		uint8_t _tx_buf[10] = {0};
		uint8_t _rx_buf[10] = {0};
		void writeBytes(uint8_t wBuf[]);
		bool readBytes(uint8_t rbuf[],uint8_t rlen,uint8_t waitTime);
};

#endif
/*************************************************
File:          BMV56T123.cpp
Author:        BEST MODULES CORP.
Description:   UART communication with the BMV56T123 and obtain the corresponding value 
Version:       V1.0.2   -- 2024-11-15
**************************************************/
#include "BMV56T123.h"

/***************************************************************************************
Description:    Constructor
parameter:
		      intPin：Interruption occurs when there are touch keys
		      rxPin: simulate serial port data receive   
		      txPin: simulate serial port data send       
Return:   None      
Others:   None  
***************************************************************************************/
BMV56T123::BMV56T123(uint8_t intPin, uint8_t rxPin, uint8_t txPin)
{
    _intPin = intPin;
    _rxPin = rxPin;
    _txPin = txPin;	
}

/*************************************************
Description: BMV56T123 initial
parameter:   void       
Return:      void     
Others:      None
*************************************************/
void BMV56T123::begin(void)
{
	pinMode(_intPin, INPUT_PULLUP);
    _softSerial = new SoftwareSerial(_rxPin,_txPin);
    _softSerial->begin(BAUDRATE); 
	delay(200);
}

/*************************************************
Description: Set play mode
Parameters:  mode: Sound source selection 
               0x00: Built-in MIDI audio source
               0x01: Custom audio source  
Return:      Implementation status
               true: Executed successfully
               false: Execution failure
Others:      None
*************************************************/
bool BMV56T123::setPlayMode(uint8_t mode)
{
	_tx_buf[0] = _CMD_SET_PLAY_MODE;
	_tx_buf[1] = mode;
	delay(5);
	writeBytes(_tx_buf);
	delay(5);
	readBytes(_rx_buf, 1, 4);
	if(_rx_buf[0] == 0xa0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*************************************************
Description: Set voice groups
Parameters:  group: Timbre group selection, Radius: 0~12(default 0) 
Return:      Implementation status
               true: Executed successfully
               false: Execution failure
Others:      None
*************************************************/
bool BMV56T123::setTimbreGroup(uint8_t group)
{
	_tx_buf[0] = _CMD_SET_VOICE;
	_tx_buf[1] = group;
	delay(5);
	writeBytes(_tx_buf); 
	delay(5);
	readBytes(_rx_buf, 1, 4);
	if(_rx_buf[0] == 0xa0)
	{
		return true;
	}
	else
	{
		return false;
	}	
}

/*************************************************
Description: Volume Setting
Parameters:  volume: Set volume, Radius: 0~15(default 0) 
Return:      Implementation status
               true: Executed successfully
               false: Execution failure
Others:      None
*************************************************/
bool BMV56T123::setVolume(uint8_t volume)
{
	_tx_buf[0] = _CMD_SET_VOLUME;
	_tx_buf[1] = volume;

	delay(5);
	writeBytes(_tx_buf); 
	delay(5);
	readBytes(_rx_buf, 1, 4);
	if(_rx_buf[0] == 0xa0)
	{
		return true;
	}
	else
	{
		return false;
	}	
}

/*************************************************
Description: sound playback
Parameters:  voice_number: Touch detection pin, Radius: 1~16 
Return:      Implementation status
               true: Executed successfully
               false: Execution failure
Others:      None
*************************************************/
bool BMV56T123::playVoice(uint8_t voice_number)
{
	_tx_buf[0] = _CMD_PLAY_VOICE;
	_tx_buf[1] = voice_number - 1;
	delay(5);
	writeBytes(_tx_buf); 
	delay(5);
	readBytes(_rx_buf, 1, 4);
	if(_rx_buf[0] == 0xa0)
	{
		return true;
	}
	else
	{
		return false;
	}	
}

/*************************************************
Description: Turn off the LED if not brighting;
Parameters:  led_number: LED serial number, Radius: 1~16 
Return:      Implementation status
               true: Executed successfully
               false: Execution failure
Others:      None 
*************************************************/
bool BMV56T123::openLED(uint8_t led_number)
{
	_tx_buf[0] = _CMD_OPEN_LED;
	_tx_buf[1] = led_number - 1;
	delay(5);
	writeBytes(_tx_buf);
	delay(5);
	readBytes(_rx_buf, 4, 4);
	if(_rx_buf[0] == 0xa0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*************************************************
Description: Turn off the LED if brighting;
Parameters:  led_number: LED serial number, Radius: 1~16 
Return:      Implementation status
               true: Executed successfully
               false: Execution failure
Others:      None  
*************************************************/
bool BMV56T123::closeLED(uint8_t led_number)
{
	_tx_buf[0] = _CMD_CLOSE_LED;
	_tx_buf[1] = led_number - 1;
	delay(5);
	writeBytes(_tx_buf);
	delay(5);
	readBytes(_rx_buf, 1, 4);
	if(_rx_buf[0] == 0xa0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*************************************************
Description: get touch key press.
Parameters:  void
Return:      key_value: Corresponding to the status of 1~16 touch detection pins
                   bit[0] : Touch detects the status of pin 1
                           bit[0] = 0:  No press
                           bit[0] = 1:  Hold down
                   .......
                   .......
                   bit[15] : Touch detects the status of pin 16
                           bit[15] = 0:  No press
                           bit[15] = 1:  Hold down                   
Others:      None  
*************************************************/
uint16_t BMV56T123::getTouchValue(void)
{
	uint16_t key_value = 0;
	uint8_t checksum = 0;
	
	_tx_buf[0] = _CMD_GET_TOUCH_VALUE;
	_tx_buf[1] = 0x00;
	delay(10);
	writeBytes(_tx_buf); 
	delay(10);
	
	readBytes(_rx_buf, 4, 4);
	for(uint8_t i = 0; i < 3; i++)
	{
		checksum += _rx_buf[i];
	}
	
	if(_rx_buf[0] == 0xa0)
	{
		if(checksum == _rx_buf[3])
		{
			key_value = _rx_buf[2];
			key_value = _rx_buf[1] + (key_value << 8);
		}
		return key_value;
	}
	else
	{
		return false;
	}
}

/********************************************************************
Description: get intterupt pin status。
Parameters:  void
Return:      INT Pin level
		             0: not intterupt occurs
		             1: intterupt occurs   
Others:      None      
********************************************************************/
uint8_t BMV56T123::getINT(void)
{
	if(digitalRead(_intPin) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/********************************************************************
Description: UART writeData
Parameters:  wBuf: Used to store received data
Return:      void      
Others:      None
********************************************************************/
void BMV56T123::writeBytes(uint8_t wBuf[])
{
	uint8_t buffer[4];
	
	buffer[0] = 0xa0;
	buffer[1] = wBuf[0];
	buffer[2] = wBuf[1];
	buffer[3] = 0;
	for(uint8_t i = 0; i < 3; i++)
	{
		buffer[3] += buffer[i];//checksum
	}
	_softSerial->flush();	
	for(uint8_t i = 0; i < 4; i++)
	{
		_softSerial->write(buffer[i]);
	}
}

/********************************************************************
Description: Read data from the module through UART communication
Parameters:  
             rbuf[]:Variables for storing Data to be obtained
             rlen:Length of data to be obtained  
             waitTime: Time OUT
Return:      Implementation status
               true: Executed successfully
               false: Execution failure
Others:      None
*********************************************************************/
bool BMV56T123::readBytes(uint8_t rbuf[],uint8_t rlen,uint8_t waitTime)
{
  uint8_t i = 0,delayCnt = 0;;
  memset(_rx_buf, 0, sizeof(uint8_t) * 10);
  
  for (i = 0; i < rlen; i++)
  {
	  while(_softSerial->available() == 0)
      {
		  if (delayCnt > waitTime)
		  {
			  return false; // Timeout error
		  }
		  delay(1);
	      delayCnt++;
	 }
	 rbuf[i] = _softSerial->read();
  }
  return true;
}
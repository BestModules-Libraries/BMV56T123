/*************************************************
File:       		BMV56T123.cpp
Author:            	Tree, BESTMODULE
Description:         BMV56T123 function
History：			
	V1.0	 -- initial version；2023-01-05；Arduino IDE : ≥v1.8.13

**************************************************/
#include "BMV56T123.h"

/***************************************************************************************
Description:    Constructor
parameter:
    Input: 
		intPin：Interruption occurs when there are touch keys
		rxPin: simulate serial port data receive   
		txPin: simulate serial port data send
    Output:         
Return:         
Others:     
***************************************************************************************/
BMV56T123::BMV56T123(uint8_t intPin, uint8_t rxPin, uint8_t txPin)
{
    _intPin = intPin;
    _rxPin = rxPin;
    _txPin = txPin;	
}
/*************************************************
Description:    BMV56T123 initial
parameter:
    Input:          
    Output:         
Return:         
Others: 
*************************************************/
void BMV56T123::begin(void)
{
	pinMode(_intPin, INPUT_PULLUP);
    _softSerial = new SoftwareSerial(_rxPin,_txPin);
    _softSerial->begin(BAUDRATE); 
	delay(200);
}
/*************************************************
Description:Set play mode
Parameters:
	mode:set play the DIY voice or MIDI voice(In-chip audio source) 
Return: 
Others: 
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
Description:Set voice groups
Parameters:
	group:0~12，default 0
Return: 
Others: 
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
Description:Volume Setting
Parameters:
	volume:0~15,max is 15,0 is Mute
Return: 
Others: 
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
Description:sound playback
Parameters:
	led_number:The sound of touch keys Number
Return: 
Others: 
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
Description:Turn off the LED if not brighting;
Parameters:
	led_number:Number of 16 LEDs on the board
Return: 
Others: 
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
Description:Turn off the LED if brighting;
Parameters:
	led_number:Number of 16 LEDs on the board
Return: 
Others:  
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
Description:get touch key press.
Parameters:
Return:  16 keys at each bit.(half word)
Others:  
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
Description:get intterupt pin status。
Parameters:
Return:
		0: not intterupt occurs
		1: intterupt occurs   
Others:      
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
Parameters:      
            wBuf: Used to store received data
            wLen: Length of data to be written
Return:      none      
Others:      
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
	    //Serial.print(buffer[i], HEX);
	    //Serial.print(' ');
	}
	//Serial.println();
}
/********************************************************************
Description: Read data from the module through UART communication
Parameters:  rbuf : Store read data   
Return:      true or false 
Others:      
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
	 //Serial.print(rbuf[i], HEX);
	 //Serial.print(' ');
  }
  //Serial.println();
  return true;
}
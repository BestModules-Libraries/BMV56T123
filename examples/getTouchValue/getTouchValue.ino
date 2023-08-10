/*************************************************
File:             getTouchValue.ino
Description:      This example shows how to read touch values.
Note:  The  baud rate  is 115200           
**************************************************/

#include <BMV56T123.h>

#define INTPin            4

BMV56T123 BM_MusicTouch(INTPin, 2, 3);

void setup() 
{
  Serial.begin(115200);
  BM_MusicTouch.begin();
    BM_MusicTouch.setPlayMode(INTERNAL_SOUND_SOURCE);
  //BM_MusicTouch.setPalyMode(EXTERNAL_SOUND_SOURCE);  //Set up playing external audio sources, 
                                                    //note that you need to add external audio sources by yourselves.
  BM_MusicTouch.setTimbreGroup(8);//Set the tone group number to 8, and the default is 0.
  BM_MusicTouch.setVolume(10);      //Set the sound size to 10, the default is 6.
}

void loop()
{
  uint16_t key_value;

  if(BM_MusicTouch.getINT() == 0)//Whether there is touch?
  {
    key_value = BM_MusicTouch.getTouchValue();
    Serial.println(key_value, BIN);//Print touch values
  }
}

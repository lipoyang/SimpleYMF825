#include "SimpleYMF825.h"

SimpleYMF825 YMF825;

int channel = 0;

void setup()
{
    YMF825.begin(IOVDD_5V);     // for 5V MCU
    //YMF825.begin(IOVDD_3V3); // for 3.3V MCU
    
    YMF825.setTone( 0, GRAND_PIANO );
    YMF825.setTone( 1, E_PIANO );
    YMF825.setTone( 2, TENOR_SAX );
    YMF825.setTone( 3, PICK_BASS );
    YMF825.setTone( 4, TNKL_BELL );
    YMF825.setTone( 5, NEW_AGE_PD );
    YMF825.setTone( 6, BRIGHT_PIANO );
    YMF825.setTone( 7, VIBES );
    YMF825.setTone( 8, CHURCH_ORGAN );
    YMF825.setTone( 9, FLUTE );
    YMF825.setTone(10, ROCK_ORGAN );
    YMF825.setTone(11, NYLON_GUITER );
    YMF825.setTone(12, SQUARE_LEAD );
    YMF825.setTone(13, SAW_LEAD );
    YMF825.setTone(14, HARPSICHORD );
    YMF825.setTone(15, HARMONICA );
    
    YMF825.setMasterVolume(32);
    
    for(int ch=0;ch<16;ch++){
        YMF825.setVolume(ch, 31);
    }
}

void loop()
{
    YMF825.keyon(channel, 4, KEY_C);
    delay(500);
    YMF825.keyoff(channel);
    delay(100);
    YMF825.keyon(channel, 4, KEY_D);
    delay(500);
    YMF825.keyoff(channel);
    delay(100);
    YMF825.keyon(channel, 4, KEY_E);
    delay(500);
    YMF825.keyoff(channel);
    delay(100);
    YMF825.keyon(channel, 4, KEY_F);
    delay(500);
    YMF825.keyoff(channel);
    delay(100);
    YMF825.keyon(channel, 4, KEY_G);
    delay(500);
    YMF825.keyoff(channel);
    delay(100);
    YMF825.keyon(channel, 4, KEY_A);
    delay(500);
    YMF825.keyoff(channel);
    delay(100);
    YMF825.keyon(channel, 4, KEY_B);
    delay(500);
    YMF825.keyoff(channel);
    delay(100);
    YMF825.keyon(channel, 5, KEY_C);
    delay(500);
    YMF825.keyoff(channel);
    delay(100);
    
    channel++;
    if(channel >= 16) channel = 0;
}

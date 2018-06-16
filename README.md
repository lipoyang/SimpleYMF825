# SimpleYMF825
Simple YMF825 driver for Arduino  

## Pin assignment
The following pin assignment is for Arduino UNO. Please check your Arduino's pinout for SPI port (MOSI, MISO, SCK).
- 9: RST_N
- 10: SS
- 11: MOSI
- 12: MISO
- 13: SCK
<br>

## SimpleYMF825::begin (drv_sel)
Begins SimpleYMF825.
### drv_sel: IO VDD voltage
- IOVDD_5V: for 5V MCU (e.g. Arduino UNO)
- IOVDD_3V3: for 3.3V MCU (e.g. Arduino DUE)
<br>

## SimpleYMF825::keyon (ch, octave, key, vol)
## SimpleYMF825::keyon (ch, octave, key)
Start the channel sound.
### ch: channel number (0-15)
### octave: octave number (1-8)
- Middle C belongs to octave 4.

### key: key name (0-11)
- KEY_C = 0
- KEY_C_SHARP = 1
- KEY_D = 2
- KEY_D_SHARP = 3
- KEY_E = 4
- KEY_F = 5
- KEY_F_SHARP = 6
- KEY_G = 7
- KEY_G_SHARP = 8
- KEY_A = 9
- KEY_A_SHARP = 10
- KEY_B = 11
### vol: channnel volume (0-31)

|vol|channnel volume|
|---|---|
|0|mute|
|1|-47.9dB|
|2|-42.6dB|
|3|-37.2dB|
|4|-33.1dB|
|5|-29.8dB|
|6|-27.0dB|
|7|-24.6dB|
|8|-22.4dB|
|9|-20.6dB|
|10|-18.9dB|
|11|-17.3dB|
|12|-15.9dB|
|13|-14.6dB|
|14|-13.4dB|
|15|-12.2dB|
|16|-11.1dB|
|17|-10.1dB|
|18|-9.2dB|
|19|-8.3dB|
|20|-7.4dB|
|21|-6.6dB|
|22|-5.8dB|
|23|-5.1dB|
|24|-4.4dB|
|25|-3.6dB|
|26|-3.0dB|
|27|-2.3dB|
|28|-1.7dB|
|29|-1.1dB|
|30|-0.6dB|
|31|0.0dB|
<br>

## SimpleYMF825::keyoff (ch)
Stop the channel sound.
### ch: channel number (0-15)
<br>

## SimpleYMF825::setTone (ch, tone)
Sets tone for the channel.
### ch: channel number (0-15)
### tone: tone number (0-15)
SimpleYMF825 provides 16 preset tones. 
These tone data are designed by [YAMAHA](https://github.com/yamaha-webmusic/ymf825board).
- GRAND_PIANO = 0
- E_PIANO = 1
- TENOR_SAX = 2
- PICK_BASS = 3
- TNKL_BELL = 4
- NEW_AGE_PD = 5
- BRIGHT_PIANO = 6
- VIBES = 7
- CHURCH_ORGAN = 8
- FLUTE = 9
- ROCK_ORGAN = 10
- NYLON_GUITER = 11
- SQUARE_LEAD = 12
- SAW_LEAD = 13
- HARPSICHORD = 14
- HARMONICA = 15
<br>

## SimpleYMF825::setKey (ch, octave, key)
Sets key for the channel.
### ch: channel number (0-15)
### octave: octave number (0-7)
### key: key name (0-11)
<br>

## SimpleYMF825::setVolume (ch, vol)
Sets volume for the channel.
### ch: channel number (0-15)
### vol: channnel volume (0-31)
<br>

## SimpleYMF825::setMasterVolume (vol)
Sets master volume. 
### vol: master volume (0-63)
The master volume applies all channnels.

|vol|master volume|
|---|---|
|0|mute|
|1|-50dB|
|2|-49dB|
|3|-48dB|
|...|...|
|61|+10dB|
|62|+11dB|
|63|+12dB|

#ifndef _SIMPLE_YMF825_H_
#define _SIMPLE_YMF825_H_

#include <stdint.h>

// tone number
#define GRAND_PIANO		0
#define E_PIANO			1
#define TENOR_SAX		2
#define PICK_BASS		3
#define TNKL_BELL		4
#define NEW_AGE_PD		5
//#define RIM_SHOT		6
//#define CASTANET		7
#define BRIGHT_PIANO	6
#define VIBES			7
#define CHURCH_ORGAN	8
#define FLUTE			9
#define ROCK_ORGAN		10
#define NYLON_GUITER	11
#define SQUARE_LEAD		12
#define SAW_LEAD		13
#define HARPSICHORD		14
#define HARMONICA		15

// key name
#define KEY_C			0
#define KEY_C_SHARP		1
#define KEY_D			2
#define KEY_D_SHARP		3
#define KEY_E			4
#define KEY_F			5
#define KEY_F_SHARP		6
#define KEY_G			7
#define KEY_G_SHARP		8
#define KEY_A			9
#define KEY_A_SHARP		10
#define KEY_B			11

// I/O voltage
#define IOVDD_5V		0
#define IOVDD_3V3		1

// number of channels
#define CH_MAX			16

// Simple YMF825 driver
class SimpleYMF825
{
public:
    void begin(int drv_sel = IOVDD_5V);
    void keyon    (int ch, int octave, int key, int vol);
    void keyon    (int ch, int octave, int key);
    void keyoff   (int ch);
    void setTone  (int ch, int tone);
    void setKey   (int ch, int octave, int key);
    void setVolume(int ch, int vol);
    void setMasterVolume(int vol);

private:
	uint8_t m_tone[CH_MAX];
};

#endif

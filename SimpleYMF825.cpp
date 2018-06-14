#include <stdint.h>
#include <SPI.h>
#include "ToneData.h"
#include "SimpleYMF825.h"

// Pin number
#define PIN_RST_N		9
#define PIN_SS			10

// FNUM table
const uint16_t FNUM[12] = 
{
	357,	// C
	378,	// C#
	401,	// D
	425,	// D#
	450,	// E
	477,	// F
	505,	// F#
	535,	// G
	567,	// G#
	601,	// A
	637,	// A#
	674,	// B
};
// FNUM table (high byte) calcurated in SimpleYMF825::begin()
uint8_t FNUMH[12];
// FNUM table (low byte) calcurated in SimpleYMF825::begin()
uint8_t FNUML[12];

// SPI write (multi bytes)
static void spi_write_bytes(uint8_t addr, uint8_t *data, int len)
{
	digitalWrite(PIN_SS, LOW);
	
	SPI.transfer(addr);
	for(int i=0; i<len; i++){
		SPI.transfer(data[i]);
	}
	
	digitalWrite(PIN_SS, HIGH);
}

// SPI write (single byte)
static void spi_write(uint8_t addr, uint8_t data)
{
	spi_write_bytes(addr, &data, 1);
}

#if 0
// SPI read (single byte)
static uint8_t spi_read(uint8_t addr)
{
	digitalWrite(PIN_SS, LOW);
	
	SPI.transfer(0x80|addr);
	uint8_t rcv = SPI.transfer(0x00);
	
	digitalWrite(PIN_SS, HIGH);
	
	return rcv;
}
#endif

// initilalize YMF825
static void init_YMF825(int drv_sel)
{
	// reset YMF825
	digitalWrite(PIN_RST_N, LOW);
	delay(1);
	digitalWrite(PIN_RST_N, HIGH);
	
	// IOVDD = 5V or 3.3V
	spi_write(29, (drv_sel & 0x01) );
	
	spi_write( 2, 0x0E ); // analog block power down (AP0)
	delay(1);
	spi_write( 0, 0x01 ); // clock enable
	spi_write( 1, 0x00 ); // reset release
	spi_write(26, 0xA3 ); // ??? (magic spell)
	delay(1);
	spi_write(26, 0x00 ); // ??? (magic spell)
	delay(30);
	spi_write( 2, 0x04 ); // analog block power down (AP1,AP3)
	delay(1);
	spi_write( 2, 0x00 ); // analog block power down (AP2)
	delay(1);
	
	spi_write(25, 0xF0 ); // master volume = +9dB ((0x00<<2):mute`(0x3F<<2):+12dB)
	spi_write(27, 0x3F ); // interpolation max
	spi_write(20, 0x00 ); // interpolation on
	spi_write( 3, 0x01 ); // analog gain = 6.5dB(default) (0:5dB, 1:6.5dB, 2:7dB, 3:7.5dB)
	
	spi_write( 8, 0xF6 ); // sequencer setting (magic spell)
	delay(21);
	spi_write( 8, 0x00 ); // sequencer reset release
	
	spi_write( 9, 0xF8 ); // sequencer volume = 0dB(max)
	spi_write(10, 0x00 ); // sequencer data size = 0 (no use)
	spi_write(23, 0x40 ); // sequencer time unit (H7bit) (no use)
	spi_write(24, 0x00 ); // sequencer time unit (L7bit) (no use)
}


// initialize YMF825's tone
static void init_tone(void)
{
	// tone parameter data size
	const int COMMON_PARAM_SIZE	= 2;
	const int OP_PARAM_SIZE		= 7;
	const int TONE_PARAM_SIZE	= COMMON_PARAM_SIZE + FM_OP_NUM * OP_PARAM_SIZE;
	const int MESSAGE_SIZE		= TONE_PARAM_SIZE * TONE_NUM + 1 + 4; // 485

	uint8_t send_data[MESSAGE_SIZE];
	
	// header
	send_data[0] = 0x80 + TONE_NUM;
	
	// register data
	for (int i=0; i<TONE_NUM; i++ )
	{
		const ToneData* td = &TONE_TABLE[i];
		uint8_t* data = &send_data[TONE_PARAM_SIZE*i + 1];
		
		// common parameter
		data[0] =  (td->voiceCommon & 0x60) >> 5;
		data[1] = ((td->voiceCommon & 0x18) << 3) | (td->voiceCommon & 0x07);
		
		// operator's parameter
		for (int j=0; j<FM_OP_NUM; j++ )
		{
			data[COMMON_PARAM_SIZE+OP_PARAM_SIZE*j+0] =  (td->opParam[j][3] << 4) |
														 (td->opParam[j][0] & 0x08) |
														((td->opParam[j][0] & 0x04)>>2);
			data[COMMON_PARAM_SIZE+OP_PARAM_SIZE*j+1] =  (td->opParam[j][4] << 4) |
														  td->opParam[j][2];
			data[COMMON_PARAM_SIZE+OP_PARAM_SIZE*j+2] =  (td->opParam[j][1] << 4) |
														  td->opParam[j][5];
			data[COMMON_PARAM_SIZE+OP_PARAM_SIZE*j+3] =  (td->opParam[j][6] << 2) |
														 (td->opParam[j][0] & 0x03);
			data[COMMON_PARAM_SIZE+OP_PARAM_SIZE*j+4] =   td->opParam[j][7];
			data[COMMON_PARAM_SIZE+OP_PARAM_SIZE*j+5] = ((td->opParam[j][8] & 0x0f) << 4) |
														((td->opParam[j][8] & 0xf0) >> 4);
			data[COMMON_PARAM_SIZE+OP_PARAM_SIZE*j+6] =  (td->opParam[j][9] << 3) |
														((td->opParam[j][0] & 0x70) >> 4);
		}
	}
	
	// footer
	send_data[TONE_PARAM_SIZE * TONE_NUM + 1] = 0x80;
	send_data[TONE_PARAM_SIZE * TONE_NUM + 2] = 0x03;
	send_data[TONE_PARAM_SIZE * TONE_NUM + 3] = 0x81;
	send_data[TONE_PARAM_SIZE * TONE_NUM + 4] = 0x80;
	
	// send data
	spi_write(8, 0xF6);
	delay(1);
	spi_write(8, 0x00);
	spi_write_bytes(7, send_data, MESSAGE_SIZE );
}

// initialize YMF825's channels
static void init_ch(void)
{
	for(uint8_t ch=0; ch<CH_MAX; ch++)
	{
		// channnel select
		spi_write( 11, ch );
		
		spi_write( 15, 0x30 ); // keyoff/mute/reset
		spi_write( 16, 0x71 ); // channel volume = -1.7dB (TODO)/ no interpolation
		spi_write( 17, 0x00 ); // no vibrato modulation
		spi_write( 18, 0x08 ); // fracency multiplier = 1.0
		spi_write( 19, 0x00 ); // fracency multiplier = 1.0
	}
}

// initialize SimpleYMF825
void SimpleYMF825::begin(int drv_sel)
{
	// initialize variables
	for(int i=0;i<CH_MAX;i++){
		m_tone[i] = 0;
	}
	for(int i=0;i<12;i++){
		FNUMH[i] = (uint8_t)((FNUM[i] & 0x0380) >> 4);
		FNUML[i] = (uint8_t) (FNUM[i] & 0x007F);
	}
	// initialize GPIO
	pinMode(PIN_RST_N, OUTPUT);
	pinMode(PIN_SS, OUTPUT);
	digitalWrite(PIN_SS, HIGH);
	
	// initialize SPI
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV8);
	SPI.setDataMode(SPI_MODE0);
	SPI.begin();
	
	// initialize YMF825
	init_YMF825(drv_sel);
	
	// initialize YMF825's tones
	init_tone();
	
	// initialize YMF825's channels
	init_ch();
}

// key on
// ch: channel number
// octave: 0...7 (Middle C belongs to octave 4)
// key: KEY_C ... KEY_B_SHARP
// vol: volume
//      0: mute
//      1...31: -47.9dB...0dB
void SimpleYMF825::keyon(int ch, int octave, int key, int vol)
{
	// channnel select
	spi_write( 11, (uint8_t)ch );
	
	// voice volume
	spi_write( 12, (uint8_t)(vol << 2) );
	
	// key
	spi_write( 13, FNUMH[key] | (uint8_t)octave );
	spi_write( 14, FNUML[key] );
	
	// key on
	spi_write( 15, 0x40 | m_tone[ch] );
}

// key on
// ch: channel number
// octave: 0...7 (Middle C belongs to octave 4)
// key: KEY_C ... KEY_B_SHARP
void SimpleYMF825::keyon(int ch, int octave, int key)
{
	// channnel select
	spi_write( 11, (uint8_t)ch );
	
	// key
	spi_write( 13, FNUMH[key] | (uint8_t)octave );
	spi_write( 14, FNUML[key] );
	
	// key on
	spi_write( 15, 0x40 | m_tone[ch] );
}

// key off
// ch: channel number
void SimpleYMF825::keyoff(int ch)
{
	// channnel select
	spi_write( 11, (uint8_t)ch );
	
	// key off
	spi_write( 15, 0x00 );
}

// set tone
// ch: channel number
// tone: tone number
void SimpleYMF825::setTone(int ch, int tone)
{
	if((ch < CH_MAX) && (tone < TONE_NUM)){
		m_tone[ch] = (uint8_t)tone;
	}
}

// set key
// ch: channel number
// octave: 0...7 (Middle C belongs to octave 4)
// key: KEY_C ... KEY_B_SHARP
void SimpleYMF825::setKey(int ch, int octave, int key)
{
	// channnel select
	spi_write( 11, (uint8_t)ch );
	
	// key
	spi_write( 13, FNUMH[key] | (uint8_t)octave );
	spi_write( 14, FNUML[key] );
}

// set volume
// ch: channel number
// vol: volume
//      0: mute
//      1...31: -47.9dB...0dB
void SimpleYMF825::setVolume(int ch, int vol)
{
	// channnel select
	spi_write( 11, (uint8_t)ch );
	
	// voice volume
	spi_write( 12, (uint8_t)(vol << 2) );
}

// set master volume
// vol: volume (0...63)
//      0: mute
//      1...63: -50dB...+12dB
void SimpleYMF825::setMasterVolume(int vol)
{
	spi_write( 25, (uint8_t)(vol << 2) );
}

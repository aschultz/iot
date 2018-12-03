// 
// 
// 

#include "TLC59711.h"
#include <SPI.h>
#include <assert.h>

static const SPISettings SpiSettings( TLC59711::CLOCK_SPD, MSBFIRST, SPI_MODE0 );

TLC59711::TLC59711( uint8_t deviceCount )
{
    numDevices = deviceCount;
    outputBufferLength = NUM_CHANNELS * numDevices;
    outputBuffer = new uint16_t[outputBufferLength];
    clear();
}

TLC59711::~TLC59711()
{
    delete[] outputBuffer;
}


void TLC59711::begin()
{
	// Init SPI bus
    SPI.begin();
}

void TLC59711::submit()
{
    // 6 bits = WRITE command = 0x25
    // 1 bit = OUTTMG = 1
    // 1 bit = EXTGCK = 0
    // 1 bit = TMGRST = 1
    // 1 bit = DSPRPT = 1
    // 1 bit = BLANK = 0
    // 7 bits = OUTB3-0 = 0x7F
    // 7 bits = OUTG3-0 = 0x7F
    // 7 bits = OUTR3-0 = 0x7F
    uint32_t command = 0x96DFFFFF;

    SPI.beginTransaction( SpiSettings );
    cli();

    // 6   bits = WRITE command
    // 26  bits = command flags
    // 192 bits = 12 x 16-bit Channels
    // -------------------------------
    // 224 bits total (size of shift register)

    // When N units of the TLC59711 are cascaded, N × 224 bits must be written from the
    // controller into the first device to control all devices. The number of cascaded devices is not limited as long as the
    // proper voltage is supplied to the device at VCC. The packets for all devices must be written again whenever the
    // data in one packet is changed.

    for (uint8_t device = 0; device < numDevices; ++device)
    {
        SPI.transfer( (uint8_t)(command >> 24) );
        SPI.transfer( (uint8_t)(command >> 16) );
        SPI.transfer( (uint8_t)(command >> 8) );
        SPI.transfer( (uint8_t)(command) );

        // Channels are written in descending order
        for (uint8_t channel = NUM_CHANNELS*device + 11; channel >= NUM_CHANNELS*device; --channel)
        {
            SPI.transfer( outputBuffer[channel >> 8] );
            SPI.transfer( outputBuffer[channel] );
        }
    }

    // Wait for at least 8x clock period for device latch
    delayMicroseconds( LATCH_DELAY );

    sei();
    SPI.endTransaction();
}

void TLC59711::setPWM( uint8_t device, uint8_t channel, uint16_t value )
{
    assert( device < numDevices );
    assert( channel < NUM_CHANNELS );

    outputBuffer[device*NUM_CHANNELS + channel] = value;
}

void TLC59711::setPWM( uint8_t channel, uint16_t value )
{
    assert( channel < NUM_CHANNELS );

    for (uint8_t device = 0; device < numDevices; ++device)
    {
        outputBuffer[device*NUM_CHANNELS + channel] = value;
    }
}

void TLC59711::setLED( uint8_t device, uint8_t ledIndex, uint16_t r, uint16_t g, uint16_t b )
{
    setPWM( device, ledIndex * 3, r );
    setPWM( device, ledIndex * 3 + 1, g );
    setPWM( device, ledIndex * 3 + 2, b );
}

void TLC59711::setLED( uint8_t ledIndex, uint16_t r, uint16_t g, uint16_t b )
{
    for (uint8_t device = 0; device < numDevices; ++device)
    {
        setPWM( device, ledIndex * 3, r );
        setPWM( device, ledIndex * 3 + 1, g );
        setPWM( device, ledIndex * 3 + 2, b );
    }
}

void TLC59711::clear()
{
    memset( outputBuffer, 0, outputBufferLength*sizeof(outputBuffer[0]) );
}

void TLC59711::clear( uint8_t device )
{
    assert( device < numDevices );

    memset( outputBuffer + device*NUM_CHANNELS, 0, NUM_CHANNELS * sizeof( outputBuffer[0] ) );
}
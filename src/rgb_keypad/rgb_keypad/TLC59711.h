// TLC59711.h

#ifndef _TLC59711_h
#define _TLC59711_h

#include <Arduino.h>

/// <summary>
/// Controller for TLC59711 RGB LED PWM drivers. Uses SPI for communication.
/// All configuration is buffered. Use the set*() and clear() commands to configure the desired state,
/// then submit() to set the state on the connected devices.
/// </summary>
class TLC59711
{
public:
    static const uint32_t       CLOCK_SPD    = 10000000; // 10Mhz
    static const unsigned int   LATCH_DELAY  = 2;        // microseconds
    static const uint8_t        NUM_CHANNELS = 12;

    /// <summary>
    /// Creates a controller for communicating with a chain of TLC59711 devices
    /// </summary>
    /// <param name="deviceCount">The number of devices chained together</param>
    TLC59711(uint8_t deviceCount);
    ~TLC59711();
    
    /// <summary>
    /// Initializes the SPI bus. Should be called in setup.
    /// </summary>
    void begin();

    /// <summary>
    /// Sets output of all channels
    /// </summary>
    /// <param name="channel">A channel index in the range [0, NUM_CHANNELS-1]</param>
    /// <param name="value">The output value</param>
    void setPWM(uint8_t channel, uint16_t value);

    /// <summary>
    /// Sets output of a device channel
    /// </summary>
    /// <param name="device">A device index in the range [0, deviceCount-1]</param>
    /// <param name="channel">A channel index in the range [0, NUM_CHANNELS-1]</param>
    /// <param name="value">The output value</param>
    void setPWM(uint8_t device, uint8_t channel, uint16_t value);

    /// <summary>
    /// Sets the intensity of the indexed R/G/B LED on all devices. This is simply
    /// treating R, G, and B as channels (ledIndex + 0/1/2).
    /// </summary>
    /// <param name="ledIndex">An index in the range [0, NUM_CHANNELS/3-1]</param>
    /// <param name="r">The intensity of red light</param>
    /// <param name="g">The intensity of green light</param>
    /// <param name="b">The intensity of blue light</param>
    void setLED(uint8_t ledIndex, uint16_t r, uint16_t g, uint16_t b);

    /// <summary>
    /// Sets the intensity of the indexed R/G/B LED. This is simply
    /// treating R, G, and B as channels (ledIndex + 0/1/2).
    /// </summary>
    /// <param name="device">A device index in the range [0, deviceCount-1]</param>
    /// <param name="ledIndex">An index in the range [0, NUM_CHANNELS/3-1]</param>
    /// <param name="r">The intensity of red light</param>
    /// <param name="g">The intensity of green light</param>
    /// <param name="b">The intensity of blue light</param>
    void setLED(uint8_t device, uint8_t ledIndex, uint16_t r, uint16_t g, uint16_t b);

    /// <summary>
    /// Sets all PWM channels for all devices to zero
    /// </summary>
    void clear();

    /// <summary>
    /// Sets all PWM channels on a device to zero
    /// </summary>
    /// <param name="device"></param>
    void clear(uint8_t device);

    /// <summary>
    /// Transmits the state of all channels to connected devices
    /// </summary>
    void submit();

private:
    uint8_t   numDevices;
	uint16_t  outputBufferLength;
    uint16_t *outputBuffer;
};

#endif


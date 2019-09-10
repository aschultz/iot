#pragma once
#include <cstdlib>
#include <cstdint>
#include <cstdbool>
#include <memory>

class I2CAdapter {
public:
    I2CAdapter(int id);
    ~I2CAdapter();

    int getHandle() { return this->handle; }

private:
    int handle;
};

class I2CDevice {
public:
    I2CDevice(int adapterId, int slaveId);

    void writeData(uint8_t *data, size_t length);
    void writeRead(uint8_t *writeBuffer, size_t writeLength, uint8_t *readBuffer, uint8_t readLength);

private:
    std::unique_ptr<I2CAdapter> adapter;
    unsigned long supportedFuncs;
    int slaveId;
};
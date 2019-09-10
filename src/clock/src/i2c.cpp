#include "../include/i2c.h"
#include "../include/util.h"

#include <cstdio>
#include <string>

#include <cerrno>
#include <cassert>
#include <system_error>

#include <unistd.h>
#include <fcntl.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>


I2CAdapter::I2CAdapter(int id) {
    std::string filename = stringFormat("/dev/i2c-%d", id);

    int file = open(filename.c_str(), O_RDWR);
    if (file < 0) {
        throw std::system_error(errno, std::system_category(), "Failed to open I2C adapter");
    }
    this->handle = file;
}

I2CAdapter::~I2CAdapter() {
    close(this->handle);
}

I2CDevice::I2CDevice(int busId, int slaveId) {
    this->adapter = std::make_unique<I2CAdapter>(busId);

    if (ioctl(this->adapter->getHandle(), I2C_FUNCS, &this->supportedFuncs) < 0) {
        throw std::system_error(errno, std::system_category(), "Failed to retrieve I2C adapter capabilities");
    }
    if ((this->supportedFuncs & I2C_FUNC_I2C) == 0) {
        // I2C_FUNC_I2C indicates that we can use I2C_RDWR to communicate with slave devices. If this
        // capability is not available, we'll need to manually set the slave ID and use read/write file commands.

        if (ioctl(this->adapter->getHandle(), I2C_SLAVE, this->slaveId) < 0) {
            throw std::system_error(errno, std::system_category(), "Failed to set I2C slave device id");
        }
    }
}

void I2CDevice::writeData(uint8_t *data, size_t length) {
    if (this->supportedFuncs & I2C_FUNC_I2C) {
        i2c_rdwr_ioctl_data i2cData = {};
        i2c_msg msg = {};

        msg.addr = this->slaveId;
        msg.flags = 0;
        msg.buf = data;
        msg.len = length;

        i2cData.msgs = &msg;
        i2cData.nmsgs = 1;

        if (ioctl(this->adapter->getHandle(), I2C_RDWR, &i2cData) < 0) {
            throw std::system_error(errno, std::system_category(), "Failed to write data to I2C slave device");
        }
    } else {
        if (write(this->adapter->getHandle(), data, length) < 0) {
            throw std::system_error(errno, std::system_category(), "Failed to write data to I2C slave device");
        }
    }
}

void I2CDevice:: writeRead(uint8_t *writeBuffer, size_t writeLength, uint8_t *readBuffer, uint8_t readLength) {
    if (this->supportedFuncs & I2C_FUNC_I2C) {
        i2c_rdwr_ioctl_data i2cData = {};
        i2c_msg msg[2] = {};

        msg[0].addr = this->slaveId;
        msg[0].flags = 0;
        msg[0].buf = writeBuffer;
        msg[0].len = writeLength;

        msg[1].addr = this->slaveId;
        msg[1].flags = I2C_M_RD;
        msg[1].buf = readBuffer;
        msg[1].len = readLength;

        i2cData.msgs = msg;
        i2cData.nmsgs = 2;

        if (ioctl(this->adapter->getHandle(), I2C_RDWR, &i2cData) < 0) {
            throw std::system_error(errno, std::system_category(), "Failed to write data to I2C slave device");;
        }
    } else {
        assert(false);
        throw std::runtime_error("Combined write/read not supported on I2C adapter");
    }
}

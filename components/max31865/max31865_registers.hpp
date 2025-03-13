#ifndef MAX31865_REGISTERS_HPP
#define MAX31865_REGISTERS_HPP

#include <cstdint>

#define PACKED __attribute__((__packed__))

namespace MAX31865 {

    struct CONFIG {
        std::uint8_t vbias : 1;
        std::uint8_t conv_mode : 1;
        std::uint8_t oneshot : 1;
        std::uint8_t nwires : 1;
        std::uint8_t fault_detect : 2;
        std::uint8_t fault_clear : 1;
        std::uint8_t mainsfilter : 1;
    } PACKED;

    struct RTD {
        std::uint16_t rtd : 15;
        std::uint8_t fault : 1;
    } PACKED;

    struct HIGH_FAULT {
        std::uint16_t high_fault : 16;
    } PACKED;

    struct LOW_FAULT {
        std::uint16_t low_fault : 16;
    } PACKED;

    struct FAULT_STATUS {
        std::uint8_t fault_status : 8;
    } PACKED;

}; // namespace MAX31865

#endif // MAX31865_REGISTERS_HPP
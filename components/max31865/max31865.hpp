#ifndef MAX31865_HPP
#define MAX31865_HPP

#include "max31865_config.hpp"
#include "max31865_registers.hpp"
#include "spi_device.hpp"
#include <optional>

namespace MAX31865 {

    struct MAX31865 {
    public:
        MAX31865() noexcept = default;
        MAX31865(SPIDevice&& spi_device,
                 std::int16_t const threshold_min,
                 std::int16_t const threshold_max,
                 NWires const nwires,
                 FaultDetect const fault_detect,
                 FaultClear const fault_clear,
                 Filter const filter,
                 ConvMode const conv_mode) noexcept;

        MAX31865(MAX31865 const& other) = delete;
        MAX31865(MAX31865&& other) noexcept = default;

        MAX31865& operator=(MAX31865 const& other) = delete;
        MAX31865& operator=(MAX31865&& other) noexcept = default;

        ~MAX31865() noexcept;

        std::optional<std::int16_t> get_resistance_raw() noexcept;
        std::optional<float> get_resistance_scaled() noexcept;

    private:
        void initialize(std::int16_t const threshold_min,
                        std::int16_t const threshold_max,
                        NWires const nwires,
                        FaultDetect const fault_detect,
                        FaultClear const fault_clear,
                        Filter const filter,
                        ConvMode const conv_mode) noexcept;
        void deinitialize() noexcept;

        void set_config_register(CONFIG const config) const noexcept;
        CONFIG get_config_register() const noexcept;

        void set_high_fault_registers(HIGH_FAULT const high_fault) const noexcept;
        HIGH_FAULT get_high_fault_registers() const noexcept;

        void set_low_fault_registers(LOW_FAULT const low_fault) const noexcept;
        LOW_FAULT get_low_fault_registers() const noexcept;

        RTD get_rtd_registers() const noexcept;

        void set_config(NWires const nwires,
                        FaultDetect const fault_detect,
                        FaultClear const fault_clear,
                        Filter const filter,
                        ConvMode const conv_mode) const noexcept;
        void set_high_fault(std::int16_t const threshold_max) const noexcept;
        void set_low_fault(std::int16_t const threshold_min) const noexcept;
        void set_vbias(bool const vbias) const noexcept;
        void start_one_shot_conversion() const noexcept;

        bool initialized_{false};

        SPIDevice spi_device_{};
    };

}; // namespace MAX31865

#endif // MAX31865_HPP

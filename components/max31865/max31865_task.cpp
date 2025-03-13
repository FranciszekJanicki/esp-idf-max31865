#include "max31865_task.hpp"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "max31865.hpp"
#include "spi_device.hpp"
#include <array>

namespace MAX31865 {

    auto constexpr TASK_TAG{"MAX31865"};
    auto task_stack = std::array<StackType_t, 4096UL>{};
    auto static_task = StaticTask_t{};

    auto max31865_spi_device = spi_device_handle_t{nullptr};
    auto constexpr MAX31865_CS = gpio_num_t::GPIO_NUM_0;

    void gpio_init() noexcept
    {
        gpio_config_t const config{.pin_bit_mask = 1UL << static_cast<std::uint32_t>(MAX31865_CS),
                                   .mode = GPIO_MODE_OUTPUT,
                                   .pull_up_en = GPIO_PULLUP_DISABLE,
                                   .pull_down_en = GPIO_PULLDOWN_DISABLE,
                                   .intr_type = GPIO_INTR_DISABLE};
        gpio_config(&config);
    }

    void spi_init() noexcept
    {
        spi_device_interface_config_t const config{.command_bits = 0,
                                                   .address_bits = 8,
                                                   .dummy_bits = 0,
                                                   .mode = 3,
                                                   .clock_source = SPI_CLK_SRC_DEFAULT,
                                                   .duty_cycle_pos = 0,
                                                   .cs_ena_pretrans = 0,
                                                   .cs_ena_posttrans = 0,
                                                   .clock_speed_hz = 5 * 1000 * 1000,
                                                   .input_delay_ns = 0,
                                                   .spics_io_num = -1,
                                                   .flags = SPI_DEVICE_HALFDUPLEX,
                                                   .queue_size = 1,
                                                   .pre_cb = nullptr,
                                                   .post_cb = nullptr};
        spi_bus_add_device(SPI3_HOST, &config, &max31865_spi_device);
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    void task(void* parameter) noexcept
    {
        spi_init();
        gpio_init();

        auto spi_device = SPIDevice{max31865_spi_device, MAX31865_CS};

        auto max31865 = MAX31865{std::move(spi_device),
                                 0,
                                 1000,
                                 NWires::TWO,
                                 FaultDetect::NO_ACTION,
                                 FaultClear::AUTO,
                                 Filter::HZ50,
                                 ConvMode::ONESHOT};

        while (true) {
            ESP_LOGI(TASK_TAG, "resistance: %f", max31865.get_resistance_scaled().value());
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }

    void start_task() noexcept
    {
        xTaskCreateStaticPinnedToCore(&task,
                                      TASK_TAG,
                                      task_stack.size(),
                                      nullptr,
                                      1,
                                      task_stack.data(),
                                      &static_task,
                                      1);
    }

}; // namespace MAX31865

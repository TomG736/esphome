#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/mc3000/mc3000_protocol.h"

#ifdef USE_ESP32

#include <esp_gattc_api.h>

namespace esphome {
namespace mc3000 {

static const uint8_t CHANNEL_COUNT = 4;

class Mc3000 : public esphome::ble_client::BLEClientNode, public PollingComponent {
 public:
  void dump_config() override;
  void setup() override;
  void update() override;
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  void set_channel_voltage(int index, sensor::Sensor *sensor) { channel_voltage_[index] = sensor; }
  void set_channel_current(int index, sensor::Sensor *sensor) { channel_current_[index] = sensor; }
  void set_channel_capacity(int index, sensor::Sensor *sensor) { channel_capacity_[index] = sensor; }
  void set_channel_temperature(int index, sensor::Sensor *sensor) { channel_temperature_[index] = sensor; }

 protected:
  void request_channel_data_(uint8_t channel);

  uint16_t char_handle_;
  std::unique_ptr<Mc3000Protocol> protocol_;
  std::array<sensor::Sensor *, CHANNEL_COUNT> channel_voltage_;
  std::array<sensor::Sensor *, CHANNEL_COUNT> channel_current_;
  std::array<sensor::Sensor *, CHANNEL_COUNT> channel_capacity_;
  std::array<sensor::Sensor *, CHANNEL_COUNT> channel_temperature_;
};

}  // namespace mc3000
}  // namespace esphome

#endif

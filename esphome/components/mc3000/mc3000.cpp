#include "mc3000.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

#ifdef USE_ESP32

namespace esphome {
namespace mc3000 {

void Mc3000::dump_config() {
  ESP_LOGCONFIG(TAG, "SkyRC MC3000");
  LOG_SENSOR(" ", "Channel 1 Voltage", this->channel_voltage_[0]);
  LOG_SENSOR(" ", "Channel 1 Current", this->channel_current_[0]);
  LOG_SENSOR(" ", "Channel 1 Capacity", this->channel_capacity_[0]);
  LOG_SENSOR(" ", "Channel 1 Temperature", this->channel_temperature_[0]);
  LOG_SENSOR(" ", "Channel 2 Voltage", this->channel_voltage_[1]);
  LOG_SENSOR(" ", "Channel 2 Current", this->channel_current_[1]);
  LOG_SENSOR(" ", "Channel 2 Capacity", this->channel_capacity_[1]);
  LOG_SENSOR(" ", "Channel 2 Temperature", this->channel_temperature_[1]);
  LOG_SENSOR(" ", "Channel 3 Voltage", this->channel_voltage_[2]);
  LOG_SENSOR(" ", "Channel 3 Current", this->channel_current_[2]);
  LOG_SENSOR(" ", "Channel 3 Capacity", this->channel_capacity_[2]);
  LOG_SENSOR(" ", "Channel 3 Temperature", this->channel_temperature_[2]);
  LOG_SENSOR(" ", "Channel 4 Voltage", this->channel_voltage_[3]);
  LOG_SENSOR(" ", "Channel 4 Current", this->channel_current_[3]);
  LOG_SENSOR(" ", "Channel 4 Capacity", this->channel_capacity_[3]);
  LOG_SENSOR(" ", "Channel 4 Temperature", this->channel_temperature_[3]);
}

void Mc3000::setup() { this->protocol_ = make_unique<Mc3000Protocol>(); }

void Mc3000::update() {
  if (this->node_state != esp32_ble_tracker::ClientState::ESTABLISHED) {
    ESP_LOGV(TAG, "[%s] Cannot poll, not connected", this->parent_->address_str().c_str());
    return;
  }

  this->request_channel_data_(0);
}

void Mc3000::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param) {
  ESP_LOGV(TAG, "[%s] gattc_event_handler: %d", this->parent_->address_str().c_str(), event);
  switch (event) {
    case ESP_GATTC_OPEN_EVT: {
      if (param->open.status == ESP_GATT_OK) {
        ESP_LOGI(TAG, "[%s] Connected successfully", this->parent_->address_str().c_str());
      }
      break;
    }

    case ESP_GATTC_SEARCH_CMPL_EVT: {
      auto *chr = this->parent_->get_characteristic(MC3000_SERVICE_UUID, MC3000_CHARACTERISTIC_UUID);
      if (chr == nullptr) {
        ESP_LOGE(TAG, "[%s] No control service found at device, not an MC3000?", this->parent_->address_str().c_str());
        break;
      }
      this->char_handle_ = chr->handle;

      auto status = esp_ble_gattc_register_for_notify(this->parent_->gattc_if, this->parent_->remote_bda, chr->handle);
      if (status) {
        ESP_LOGW(TAG, "[%s] esp_ble_gattc_register_for_notify failed, status=%d", this->parent_->address_str().c_str(),
                 status);
      }
      break;
    }

    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      this->node_state = esp32_ble_tracker::ClientState::ESTABLISHED;
      this->update();
      break;
    }

    case ESP_GATTC_NOTIFY_EVT: {
      if (param->notify.handle != this->char_handle_)
        break;

      auto *packet = this->protocol_->decode(param->notify.value, param->notify.value_len);
      if (packet == nullptr)
        break;

      if (packet->command == CMD_CHANNEL_DATA) {
        int channel = packet->channel_data.channel;
        if (channel < CHANNEL_COUNT) {
          if (this->channel_voltage_[channel] != nullptr)
            this->channel_voltage_[channel]->publish_state(packet->channel_data.voltage / 1000.0f);
          if (this->channel_current_[channel] != nullptr)
            this->channel_current_[channel]->publish_state(packet->channel_data.current / 1000.0f);
          if (this->channel_capacity_[channel] != nullptr)
            this->channel_capacity_[channel]->publish_state(packet->channel_data.capacity);
          if (this->channel_temperature_[channel] != nullptr)
            this->channel_temperature_[channel]->publish_state(packet->channel_data.temperature);
        }

        if (channel < CHANNEL_COUNT - 1) {
          this->request_channel_data_(channel + 1);
        }
      }
      break;
    }

    case ESP_GATTC_DISCONNECT_EVT: {
      ESP_LOGI(TAG, "[%s] Disconnected", this->parent_->address_str().c_str());
      this->node_state = esp32_ble_tracker::ClientState::IDLE;
      for (int i = 0; i < CHANNEL_COUNT; i++) {
        if (this->channel_voltage_[i] != nullptr)
          this->channel_voltage_[i]->publish_state(NAN);
        if (this->channel_current_[i] != nullptr)
          this->channel_current_[i]->publish_state(NAN);
        if (this->channel_capacity_[i] != nullptr)
          this->channel_capacity_[i]->publish_state(NAN);
        if (this->channel_temperature_[i] != nullptr)
          this->channel_temperature_[i]->publish_state(NAN);
      }
      break;
    }

    default:
      break;
  }
}

void Mc3000::request_channel_data_(uint8_t channel) {
  ESP_LOGV(TAG, "[%s] Requesting data for channel %d", this->parent_->address_str().c_str(), channel);
  auto *packet = this->protocol_->encode_channel_data_request(channel);
  auto status =
      esp_ble_gattc_write_char(this->parent_->gattc_if, this->parent_->conn_id, this->char_handle_, sizeof(*packet),
                               (uint8_t *) packet, ESP_GATT_WRITE_TYPE_RSP, ESP_GATT_AUTH_REQ_NONE);
  if (status) {
    ESP_LOGW(TAG, "[%s] esp_ble_gattc_write_char failed, status=%d", this->parent_->address_str().c_str(), status);
  }
}

}  // namespace mc3000
}  // namespace esphome

#endif

#include "mc3000_protocol.h"
#include <cstring>
#include <cstdio>

namespace esphome {
namespace mc3000 {

uint8_t Mc3000Protocol::calculate_checksum_(const uint8_t *data, size_t length) {
  uint8_t checksum = 0;
  for (int i = 0; i < length; i++)
    checksum += data[i];
  return checksum;
}

Mc3000Packet *Mc3000Protocol::encode(uint8_t command, const uint8_t *payload, size_t length) {
  memset(&this->packet_, 0, sizeof(this->packet_));

  this->packet_.magic = CMD_MAGIC;
  this->packet_.command = command;
  memcpy(this->packet_.payload, payload, std::min(length, (size_t) sizeof(this->packet_.payload)));
  this->packet_.checksum = this->calculate_checksum_((uint8_t *) &this->packet_, sizeof(this->packet_));

  ESP_LOGV(TAG, "Encoded packet: %s", format_hex_pretty((uint8_t *) &this->packet_, sizeof(this->packet_)).c_str());

  return &this->packet_;
}

Mc3000Packet *Mc3000Protocol::encode_channel_data_request(uint8_t channel) {
  return this->encode(CMD_CHANNEL_DATA, &channel, sizeof(channel));
}

Mc3000Packet *Mc3000Protocol::decode(const uint8_t *data, size_t length) {
  ESP_LOGV(TAG, "Decoding packet: %s", format_hex_pretty(data, length).c_str());

  // We only allow single-packet messages here, because we would need a larger buffer to reassemble them.
  // Also, there is currently no need for multi-packet messages like voltage curves.
  if (length < 3 || data[0] != CMD_MAGIC || this->calculate_checksum_(data, length - 1) != data[length - 1]) {
    ESP_LOGV(TAG, "Dropping invalid packet");
    return nullptr;
  }

  memset(&this->packet_, 0, sizeof(this->packet_));
  memcpy(&this->packet_, data, std::min(length, (size_t) sizeof(this->packet_)));

  return &this->packet_;
};

}  // namespace mc3000
}  // namespace esphome

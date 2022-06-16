#pragma once

#include "esphome/core/log.h"
#include "esphome/core/datatypes.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace mc3000 {

static const char *const TAG = "mc3000";

static const uint16_t MC3000_SERVICE_UUID = 0xFFE0;
static const uint16_t MC3000_CHARACTERISTIC_UUID = 0xFFE1;

static const uint8_t CMD_MAGIC = 0x0F;
static const uint8_t CMD_CHANNEL_DATA = 0x55;
static const uint8_t CMD_VOLTAGE_CURVE = 0x56;
static const uint8_t CMD_VERSION_INFO = 0x57;
static const uint8_t CMD_BASIC_DATA = 0x61;
static const uint8_t CMD_START_CHARGE = 0x05;
static const uint8_t CMD_STOP_CHARGE = 0xFE;

struct Mc3000ChannelData {
  uint8_t channel;
  uint8_t type;
  uint8_t mode;
  uint8_t count;
  uint8_t status;
  uint16_be_t time;
  uint16_be_t voltage;
  uint16_be_t current;
  uint16_be_t capacity;
  uint8_t temperature;
  uint16_be_t resistance;
  uint8_t leds;
} __attribute__((packed));

struct Mc3000VersionInfo {
  uint8_t unknown1[12];
  uint8_t fw_version_major;
  uint8_t fw_version_minor;
  uint8_t hw_version;
  uint8_t unknown2[2];
} __attribute__((packed));

struct Mc3000BasicData {
  uint8_t temp_unit;
  uint8_t system_beep;
  uint8_t display;
  uint8_t screensaver;
  uint8_t cooling_fan;
  uint16_be_t input_voltage;
} __attribute__((packed));

struct Mc3000Packet {
  uint8_t magic;
  uint8_t command;
  union {
    uint8_t payload[17];
    Mc3000ChannelData channel_data;
    Mc3000VersionInfo version_info;
    Mc3000BasicData basic_data;
  } __attribute__((packed));
  uint8_t checksum;
} __attribute__((packed));

class Mc3000Protocol {
 public:
  Mc3000Packet *encode_channel_data_request(uint8_t channel);

  Mc3000Packet *encode(uint8_t command, const uint8_t *payload, size_t length);
  Mc3000Packet *decode(const uint8_t *data, size_t length);

 protected:
  uint8_t calculate_checksum_(const uint8_t *data, size_t length);
  void set_checksum_();
  bool check_checksum_();
  Mc3000Packet packet_;
};

}  // namespace mc3000
}  // namespace esphome

#include "es7210.h"

namespace esphome {
namespace es7210 {

// clock coefficient lookup table

static const ES7210Coefficient ES7210_COEFFICIENTS[] = {
    // clang-format off

  //    mclk      lrck    ss_ds adc_div  dll  doubler osr  mclk_src  lrckh   lrckl
  /* 8k */
  {12288000,  8000,  0x00,  0x03,  0x01,  0x00,  0x20,  0x00,    0x06,  0x00},
  {16384000,  8000,  0x00,  0x04,  0x01,  0x00,  0x20,  0x00,    0x08,  0x00},
  {19200000,  8000,  0x00,  0x1e,  0x00,  0x01,  0x28,  0x00,    0x09,  0x60},
  {4096000,   8000,  0x00,  0x01,  0x01,  0x00,  0x20,  0x00,    0x02,  0x00},

  /* 11.025k */
  {11289600,  11025,  0x00,  0x02,  0x01,  0x00,  0x20,  0x00,    0x01,  0x00},

  /* 12k */
  {12288000,  12000,  0x00,  0x02,  0x01,  0x00,  0x20,  0x00,    0x04,  0x00},
  {19200000,  12000,  0x00,  0x14,  0x00,  0x01,  0x28,  0x00,    0x06,  0x40},

  /* 16k */
  {4096000,   16000,  0x00,  0x01,  0x01,  0x01,  0x20,  0x00,    0x01,  0x00},
  {19200000,  16000,  0x00,  0x0a,  0x00,  0x00,  0x1e,  0x00,    0x04,  0x80},
  {16384000,  16000,  0x00,  0x02,  0x01,  0x00,  0x20,  0x00,    0x04,  0x00},
  {12288000,  16000,  0x00,  0x03,  0x01,  0x01,  0x20,  0x00,    0x03,  0x00},

  /* 22.05k */
  {11289600,  22050,  0x00,  0x01,  0x01,  0x00,  0x20,  0x00,    0x02,  0x00},

  /* 24k */
  {12288000,  24000,  0x00,  0x01,  0x01,  0x00,  0x20,  0x00,    0x02,  0x00},
  {19200000,  24000,  0x00,  0x0a,  0x00,  0x01,  0x28,  0x00,    0x03,  0x20},

  /* 32k */
  {12288000,  32000,  0x00,  0x03,  0x00,  0x00,  0x20,  0x00,    0x01,  0x80},
  {16384000,  32000,  0x00,  0x01,  0x01,  0x00,  0x20,  0x00,    0x02,  0x00},
  {19200000,  32000,  0x00,  0x05,  0x00,  0x00,  0x1e,  0x00,    0x02,  0x58},

  /* 44.1k */
  {11289600,  44100,  0x00,  0x01,  0x01,  0x01,  0x20,  0x00,    0x01,  0x00},

  /* 48k */
  {12288000,  48000,  0x00,  0x01,  0x01,  0x01,  0x20,  0x00,    0x01,  0x00},
  {19200000,  48000,  0x00,  0x05,  0x00,  0x01,  0x28,  0x00,    0x01,  0x90},

  /* 64k */
  {16384000,  64000,  0x01,  0x01,  0x01,  0x00,  0x20,  0x00,    0x01,  0x00},
  {19200000,  64000,  0x00,  0x05,  0x00,  0x01,  0x1e,  0x00,    0x01,  0x2c},

  /* 88.2k */
  {11289600,  88200,  0x01,  0x01,  0x01,  0x01,  0x20,  0x00,    0x00,  0x80},

  /* 96k */
  {12288000,  96000,  0x01,  0x01,  0x01,  0x01,  0x20,  0x00,    0x00,  0x80},
  {19200000,  96000,  0x01,  0x05,  0x00,  0x01,  0x28,  0x00,    0x00,  0xc8},
    // clang-format on
};

const ES7210Coefficient *ES7210Component::get_coefficient(uint32_t mclk, uint32_t lrck) {
  for (const auto &coefficient : ES7210_COEFFICIENTS) {
    if (coefficient.mclk == mclk && coefficient.lrck == lrck) {
      return &coefficient;
    }
  }
  return nullptr;
}

}  // namespace es7210
}  // namespace esphome

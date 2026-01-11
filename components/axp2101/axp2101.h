#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace axp2101 {

// Register map (subset)
static const uint8_t AXP2101_REG_DCDC_EN = 0x10;
static const uint8_t AXP2101_REG_LDO_EN = 0x12;
static const uint8_t AXP2101_REG_LDO_EN2 = 0x13;

// Bit definitions
// DCDC_EN (0x10)
static const uint8_t AXP2101_DCDC3_EN_BIT = 1 << 2;

// LDO_EN (0x12)
static const uint8_t AXP2101_ALDO1_EN_BIT = 1 << 0;
static const uint8_t AXP2101_ALDO2_EN_BIT = 1 << 1;
static const uint8_t AXP2101_ALDO3_EN_BIT = 1 << 2;
static const uint8_t AXP2101_ALDO4_EN_BIT = 1 << 3;

// LDO_EN2 (0x13)
static const uint8_t AXP2101_BLDO1_EN_BIT = 1 << 0;

enum class AXP2101Rail {
  DC3,
  ALDO1,
  ALDO2,
  ALDO3,
  ALDO4,
  BLDO1,
};

class AXP2101Component : public Component {
 public:
  void set_i2c(i2c::I2CDevice *dev) { this->i2c_dev_ = dev; }

  /// Read current enable state from registers.
  bool get_rail_enabled(AXP2101Rail rail);

  /// Set enable state (read-modify-write) and read back to confirm.
  bool set_rail_enabled(AXP2101Rail rail, bool enabled);

  bool is_failed() const { return this->failed_; }

 protected:
  i2c::I2CDevice *i2c_dev_{nullptr};
  bool failed_{false};

  bool read_u8_(uint8_t reg, uint8_t &value);
  bool write_u8_(uint8_t reg, uint8_t value);
  bool update_bits_(uint8_t reg, uint8_t mask, bool set);
};

}  // namespace axp2101
}  // namespace esphome

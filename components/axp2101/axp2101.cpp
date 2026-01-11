#include "axp2101.h"

namespace esphome {
namespace axp2101 {

bool AXP2101Component::read_u8_(uint8_t reg, uint8_t &value) {
  if (this->i2c_dev_ == nullptr) {
    this->failed_ = true;
    return false;
  }
  if (!this->i2c_dev_->read_byte(reg, &value)) {
    this->failed_ = true;
    return false;
  }
  return true;
}

bool AXP2101Component::write_u8_(uint8_t reg, uint8_t value) {
  if (this->i2c_dev_ == nullptr) {
    this->failed_ = true;
    return false;
  }
  if (!this->i2c_dev_->write_byte(reg, value)) {
    this->failed_ = true;
    return false;
  }
  return true;
}

bool AXP2101Component::update_bits_(uint8_t reg, uint8_t mask, bool set) {
  uint8_t v;
  if (!read_u8_(reg, v))
    return false;
  uint8_t nv = set ? (uint8_t) (v | mask) : (uint8_t) (v & ~mask);
  if (nv == v)
    return true;  // no-op
  if (!write_u8_(reg, nv))
    return false;
  // read-back verify
  uint8_t rb;
  if (!read_u8_(reg, rb))
    return false;
  return (rb & mask) == (set ? mask : 0);
}

static inline void rail_to_reg_mask(AXP2101Rail rail, uint8_t &reg, uint8_t &mask) {
  switch (rail) {
    case AXP2101Rail::DC3:
      reg = AXP2101_REG_DCDC_EN;
      mask = AXP2101_DCDC3_EN_BIT;
      return;
    case AXP2101Rail::ALDO1:
      reg = AXP2101_REG_LDO_EN;
      mask = AXP2101_ALDO1_EN_BIT;
      return;
    case AXP2101Rail::ALDO2:
      reg = AXP2101_REG_LDO_EN;
      mask = AXP2101_ALDO2_EN_BIT;
      return;
    case AXP2101Rail::ALDO3:
      reg = AXP2101_REG_LDO_EN;
      mask = AXP2101_ALDO3_EN_BIT;
      return;
    case AXP2101Rail::ALDO4:
      reg = AXP2101_REG_LDO_EN;
      mask = AXP2101_ALDO4_EN_BIT;
      return;
    case AXP2101Rail::BLDO1:
      reg = AXP2101_REG_LDO_EN2;
      mask = AXP2101_BLDO1_EN_BIT;
      return;
  }
  reg = AXP2101_REG_LDO_EN;
  mask = 0;
}

bool AXP2101Component::get_rail_enabled(AXP2101Rail rail) {
  uint8_t reg, mask;
  rail_to_reg_mask(rail, reg, mask);
  if (mask == 0)
    return false;
  uint8_t v;
  if (!read_u8_(reg, v))
    return false;
  return (v & mask) != 0;
}

bool AXP2101Component::set_rail_enabled(AXP2101Rail rail, bool enabled) {
  uint8_t reg, mask;
  rail_to_reg_mask(rail, reg, mask);
  if (mask == 0)
    return false;
  return update_bits_(reg, mask, enabled);
}

}  // namespace axp2101
}  // namespace esphome

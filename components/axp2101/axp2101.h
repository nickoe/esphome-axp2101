#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace axp2101 {

class AXP2101Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;

  void set_dc3_enable(bool enable) { this->dc3_enable_ = enable; }
  void set_dc3_voltage_mv(uint16_t mv) { this->dc3_voltage_mv_ = mv; this->dc3_voltage_set_ = true; }

  void set_aldo1_enable(bool enable) { this->aldo1_enable_ = enable; }
  void set_aldo1_voltage_mv(uint16_t mv) { this->aldo1_voltage_mv_ = mv; this->aldo1_voltage_set_ = true; }

  void set_aldo2_enable(bool enable) { this->aldo2_enable_ = enable; }
  void set_aldo2_voltage_mv(uint16_t mv) { this->aldo2_voltage_mv_ = mv; this->aldo2_voltage_set_ = true; }

  void set_aldo3_enable(bool enable) { this->aldo3_enable_ = enable; this->aldo3_present_ = true; }
  void set_aldo3_voltage_mv(uint16_t mv) { this->aldo3_voltage_mv_ = mv; this->aldo3_voltage_set_ = true; this->aldo3_present_ = true; }

  void set_aldo4_enable(bool enable) { this->aldo4_enable_ = enable; }
  void set_aldo4_voltage_mv(uint16_t mv) { this->aldo4_voltage_mv_ = mv; this->aldo4_voltage_set_ = true; }

  void set_bldo1_enable(bool enable) { this->bldo1_enable_ = enable; this->bldo1_present_ = true; }
  void set_bldo1_voltage_mv(uint16_t mv) { this->bldo1_voltage_mv_ = mv; this->bldo1_voltage_set_ = true; this->bldo1_present_ = true; }

 protected:
  // Only touch rails when *_enable is true (safer default)
  bool dc3_enable_{false};
  bool aldo1_enable_{false};
  bool aldo2_enable_{false};
  bool aldo3_enable_{false};
  bool aldo4_enable_{false};
  bool bldo1_enable_{false};

  bool aldo3_present_{false};
  bool bldo1_present_{false};

  uint16_t dc3_voltage_mv_{0};
  uint16_t aldo1_voltage_mv_{0};
  uint16_t aldo2_voltage_mv_{0};
  uint16_t aldo3_voltage_mv_{0};
  uint16_t aldo4_voltage_mv_{0};
  uint16_t bldo1_voltage_mv_{0};

  bool dc3_voltage_set_{false};
  bool aldo1_voltage_set_{false};
  bool aldo2_voltage_set_{false};
  bool aldo3_voltage_set_{false};
  bool aldo4_voltage_set_{false};
  bool bldo1_voltage_set_{false};

  // Register helpers
  bool read_u8_(uint8_t reg, uint8_t &value);
  bool write_u8_(uint8_t reg, uint8_t value);
  bool update_bits_(uint8_t reg, uint8_t mask, bool set);

  // Rail helpers
  bool set_dc3_voltage_reg_(uint16_t mv);
  bool set_ldo_voltage_reg_(uint8_t reg, uint16_t mv);

  void apply_dc3_();
  void apply_ldo_(bool enable, uint8_t enable_bit, bool voltage_set, uint16_t mv, uint8_t volt_reg);

  // Registers (from lewisxhe/XPowersLib @ e087e81657e7dd3ec35dac30f84115724e8b81ad)
  static constexpr uint8_t REG_DC_ONOFF_DVM_CTRL = 0x80;
  static constexpr uint8_t REG_DC_VOL2_CTRL = 0x84;

  static constexpr uint8_t REG_LDO_ONOFF_CTRL0 = 0x90;
  static constexpr uint8_t REG_LDO_VOL0_CTRL = 0x92;  // ALDO1
  static constexpr uint8_t REG_LDO_VOL1_CTRL = 0x93;  // ALDO2
  static constexpr uint8_t REG_LDO_VOL2_CTRL = 0x94;  // ALDO3
  static constexpr uint8_t REG_LDO_VOL3_CTRL = 0x95;  // ALDO4
  static constexpr uint8_t REG_LDO_VOL4_CTRL = 0x96;  // BLDO1

  // Enable bits
  static constexpr uint8_t BIT_DC3_EN = 1 << 2;  // in REG_DC_ONOFF_DVM_CTRL

  static constexpr uint8_t BIT_ALDO1_EN = 1 << 0;  // in REG_LDO_ONOFF_CTRL0
  static constexpr uint8_t BIT_ALDO2_EN = 1 << 1;
  static constexpr uint8_t BIT_ALDO3_EN = 1 << 2;
  static constexpr uint8_t BIT_ALDO4_EN = 1 << 3;
  static constexpr uint8_t BIT_BLDO1_EN = 1 << 4;
};

}  // namespace axp2101
}  // namespace esphome

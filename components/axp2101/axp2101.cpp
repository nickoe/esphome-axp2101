#include "axp2101.h"
#include "esphome/core/log.h"

namespace esphome {
namespace axp2101 {

static const char *const TAG = "axp2101";

bool AXP2101Component::read_u8_(uint8_t reg, uint8_t &value) {
  return this->read_byte(reg, &value);
}

bool AXP2101Component::write_u8_(uint8_t reg, uint8_t value) {
  return this->write_byte(reg, value);
}

bool AXP2101Component::update_bits_(uint8_t reg, uint8_t mask, bool set) {
  uint8_t v;
  if (!this->read_u8_(reg, v)) {
    ESP_LOGW(TAG, "Failed to read reg 0x%02X", reg);
    return false;
  }
  uint8_t nv = set ? (uint8_t) (v | mask) : (uint8_t) (v & ~mask);
  if (nv == v)
    return true;
  if (!this->write_u8_(reg, nv)) {
    ESP_LOGW(TAG, "Failed to write reg 0x%02X", reg);
    return false;
  }
  return true;
}

// ALDO/BLDO voltage encoding: low 5 bits = (mv - 500) / 100, preserve upper 3 bits.
bool AXP2101Component::set_ldo_voltage_reg_(uint8_t reg, uint16_t mv) {
  if (mv < 500 || mv > 3600) {
    ESP_LOGW(TAG, "LDO voltage %u mV out of supported range (500..3600)", mv);
    return false;
  }

  uint8_t cur;
  if (!this->read_u8_(reg, cur)) {
    ESP_LOGW(TAG, "Failed to read LDO volt reg 0x%02X", reg);
    return false;
  }

  uint8_t step = (uint8_t) ((mv - 500) / 100);
  if (step > 0x1F)
    step = 0x1F;

  uint8_t nv = (cur & 0xE0) | (step & 0x1F);
  if (!this->write_u8_(reg, nv)) {
    ESP_LOGW(TAG, "Failed to write LDO volt reg 0x%02X", reg);
    return false;
  }
  return true;
}

// DC3 voltage encoding (XPowersLib setDC3Voltage behavior):
//  - 500..1200 mV step 10: reg = (mv-500)/10
//  - 1220..1540 mV step 20: reg = base 71 + (mv-1220)/20
//  - 1600..3400 mV step 100: reg = base 88 + (mv-1600)/100
// Preserve bit7 of REG_DC_VOL2_CTRL.
bool AXP2101Component::set_dc3_voltage_reg_(uint16_t mv) {
  uint8_t cur;
  if (!this->read_u8_(REG_DC_VOL2_CTRL, cur)) {
    ESP_LOGW(TAG, "Failed to read DC3 volt reg 0x%02X", REG_DC_VOL2_CTRL);
    return false;
  }

  uint8_t code;
  if (mv >= 500 && mv <= 1200) {
    code = (uint8_t) ((mv - 500) / 10);
  } else if (mv >= 1220 && mv <= 1540) {
    code = (uint8_t) (71 + (mv - 1220) / 20);
  } else if (mv >= 1600 && mv <= 3400) {
    code = (uint8_t) (88 + (mv - 1600) / 100);
  } else {
    ESP_LOGW(TAG, "DC3 voltage %u mV out of supported ranges (500-1200/1220-1540/1600-3400)", mv);
    return false;
  }

  uint8_t nv = (cur & 0x80) | (code & 0x7F);
  if (!this->write_u8_(REG_DC_VOL2_CTRL, nv)) {
    ESP_LOGW(TAG, "Failed to write DC3 volt reg 0x%02X", REG_DC_VOL2_CTRL);
    return false;
  }
  return true;
}

void AXP2101Component::apply_dc3_() {
  if (!this->dc3_enable_)
    return;

  if (this->dc3_voltage_set_) {
    (void) this->set_dc3_voltage_reg_(this->dc3_voltage_mv_);
  }
  (void) this->update_bits_(REG_DC_ONOFF_DVM_CTRL, BIT_DC3_EN, true);
}

void AXP2101Component::apply_ldo_(bool enable, uint8_t enable_bit, bool voltage_set, uint16_t mv, uint8_t volt_reg) {
  if (!enable)
    return;

  if (voltage_set) {
    (void) this->set_ldo_voltage_reg_(volt_reg, mv);
  }
  (void) this->update_bits_(REG_LDO_ONOFF_CTRL0, enable_bit, true);
}

void AXP2101Component::setup() {
  // Safer default: do not disable anything; only apply rails that are explicitly enabled.
  this->apply_dc3_();
  this->apply_ldo_(this->aldo1_enable_, BIT_ALDO1_EN, this->aldo1_voltage_set_, this->aldo1_voltage_mv_, REG_LDO_VOL0_CTRL);
  this->apply_ldo_(this->aldo2_enable_, BIT_ALDO2_EN, this->aldo2_voltage_set_, this->aldo2_voltage_mv_, REG_LDO_VOL1_CTRL);
  if (this->aldo3_present_)
    this->apply_ldo_(this->aldo3_enable_, BIT_ALDO3_EN, this->aldo3_voltage_set_, this->aldo3_voltage_mv_, REG_LDO_VOL2_CTRL);
  this->apply_ldo_(this->aldo4_enable_, BIT_ALDO4_EN, this->aldo4_voltage_set_, this->aldo4_voltage_mv_, REG_LDO_VOL3_CTRL);
  if (this->bldo1_present_)
    this->apply_ldo_(this->bldo1_enable_, BIT_BLDO1_EN, this->bldo1_voltage_set_, this->bldo1_voltage_mv_, REG_LDO_VOL4_CTRL);
}

void AXP2101Component::dump_config() {
  ESP_LOGCONFIG(TAG, "AXP2101:");
  LOG_I2C_DEVICE(this);

  ESP_LOGCONFIG(TAG, "  DC3: enable=%s voltage_mv=%u%s", YESNO(this->dc3_enable_), this->dc3_voltage_mv_,
                this->dc3_voltage_set_ ? "" : " (unchanged)");

  ESP_LOGCONFIG(TAG, "  ALDO1: enable=%s voltage_mv=%u%s", YESNO(this->aldo1_enable_), this->aldo1_voltage_mv_,
                this->aldo1_voltage_set_ ? "" : " (unchanged)");
  ESP_LOGCONFIG(TAG, "  ALDO2: enable=%s voltage_mv=%u%s", YESNO(this->aldo2_enable_), this->aldo2_voltage_mv_,
                this->aldo2_voltage_set_ ? "" : " (unchanged)");
  if (this->aldo3_present_)
    ESP_LOGCONFIG(TAG, "  ALDO3: enable=%s voltage_mv=%u%s", YESNO(this->aldo3_enable_), this->aldo3_voltage_mv_,
                  this->aldo3_voltage_set_ ? "" : " (unchanged)");
  ESP_LOGCONFIG(TAG, "  ALDO4: enable=%s voltage_mv=%u%s", YESNO(this->aldo4_enable_), this->aldo4_voltage_mv_,
                this->aldo4_voltage_set_ ? "" : " (unchanged)");
  if (this->bldo1_present_)
    ESP_LOGCONFIG(TAG, "  BLDO1: enable=%s voltage_mv=%u%s", YESNO(this->bldo1_enable_), this->bldo1_voltage_mv_,
                  this->bldo1_voltage_set_ ? "" : " (unchanged)");
}

}  // namespace axp2101
}  // namespace esphome

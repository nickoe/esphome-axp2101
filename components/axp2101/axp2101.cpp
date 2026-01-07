#include "axp2101.h"
#include "esphome/core/log.h"

namespace esphome {
namespace axp2101 {

static const char *const TAG = "axp2101";

// Register addresses (subset)
static constexpr uint8_t AXP2101_REG_PWROK_STATUS = 0x80;
static constexpr uint8_t AXP2101_REG_CHG_STATUS = 0x84;
static constexpr uint8_t AXP2101_REG_DCDC_ENABLE = 0x90;
static constexpr uint8_t AXP2101_REG_DCDC1_VSET = 0x92;
static constexpr uint8_t AXP2101_REG_DCDC2_VSET = 0x93;
static constexpr uint8_t AXP2101_REG_DCDC3_VSET = 0x94;
static constexpr uint8_t AXP2101_REG_DCDC4_VSET = 0x95;
static constexpr uint8_t AXP2101_REG_DCDC5_VSET = 0x96;

// These helpers mirror common AXP2101 step tables. If the repository already uses different
// tables elsewhere, these are meant as a best-effort debug decode.
static float decode_dcdc_voltage_v(uint8_t reg, int channel) {
  // Many AXP PMICs use 10 mV steps with different min per channel.
  // AXP2101: typical mins:
  // DCDC1: 1.5V..3.4V (10mV)
  // DCDC2: 0.5V..1.2V (10mV) then 1.22..1.54 (20mV) (varies by datasheet rev)
  // DCDC3/4/5: 0.5V..1.4V (10mV) then above (20mV) (varies)
  // For debug, we provide simple decode for common 10mV linear encodings.
  float base = 0.0f;
  float step = 0.01f;
  uint8_t code = reg & 0x7F;

  switch (channel) {
    case 1:
      base = 1.5f;
      step = 0.01f;
      break;
    default:
      base = 0.5f;
      step = 0.01f;
      break;
  }

  return base + (float) code * step;
}

bool AXP2101Component::read_reg_(uint8_t reg, uint8_t *value) {
  // This component in this repo uses direct i2c read/write on Wire.
  // Keep compatible with existing implementation.
  if (value == nullptr)
    return false;

  Wire.beginTransmission(this->i2c_address_);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0)
    return false;

  uint8_t read = Wire.requestFrom((int) this->i2c_address_, 1);
  if (read != 1)
    return false;

  *value = Wire.read();
  return true;
}

bool AXP2101Component::read_regs_(uint8_t reg, uint8_t *data, size_t len) {
  if (data == nullptr || len == 0)
    return false;

  Wire.beginTransmission(this->i2c_address_);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0)
    return false;

  uint8_t read = Wire.requestFrom((int) this->i2c_address_, (int) len);
  if (read != (uint8_t) len)
    return false;

  for (size_t i = 0; i < len; i++) {
    data[i] = Wire.read();
  }
  return true;
}

bool AXP2101Component::write_reg_(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(this->i2c_address_);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission(true) == 0;
}

void AXP2101Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up AXP2101...");

  if (this->debug_dump_) {
    this->debug_dump_registers_();
  }
}

void AXP2101Component::dump_config() {
  ESP_LOGCONFIG(TAG, "AXP2101:");
  ESP_LOGCONFIG(TAG, "  I2C Address: 0x%02X", this->i2c_address_);
  ESP_LOGCONFIG(TAG, "  Debug dump: %s", this->debug_dump_ ? "true" : "false");
}

void AXP2101Component::debug_dump_registers_() {
  uint8_t v80{0}, v84{0}, v90{0};
  uint8_t vset[5]{0};

  bool ok = true;
  ok &= this->read_reg_(AXP2101_REG_PWROK_STATUS, &v80);
  ok &= this->read_reg_(AXP2101_REG_CHG_STATUS, &v84);
  ok &= this->read_reg_(AXP2101_REG_DCDC_ENABLE, &v90);
  ok &= this->read_regs_(AXP2101_REG_DCDC1_VSET, vset, sizeof(vset));

  if (!ok) {
    ESP_LOGW(TAG, "Debug dump: failed to read one or more registers");
    return;
  }

  ESP_LOGD(TAG, "AXP2101 register dump:");
  ESP_LOGD(TAG, "  0x80 (PWROK/Status): 0x%02X", v80);
  ESP_LOGD(TAG, "  0x84 (Charge/Status): 0x%02X", v84);
  ESP_LOGD(TAG, "  0x90 (DCDC enable): 0x%02X", v90);
  ESP_LOGD(TAG, "  0x92-0x96 (DCDC1-5 VSET): %02X %02X %02X %02X %02X", vset[0], vset[1], vset[2], vset[3], vset[4]);

  // Decode enables (best-effort): bit0..bit4 map to DCDC1..DCDC5.
  ESP_LOGD(TAG, "Decoded:");
  for (int ch = 1; ch <= 5; ch++) {
    bool en = (v90 & (1u << (ch - 1))) != 0;
    float volts = decode_dcdc_voltage_v(vset[ch - 1], ch);
    ESP_LOGD(TAG, "  DCDC%d: %s, vset=0x%02X (~%.2f V)", ch, en ? "EN" : "DIS", vset[ch - 1], volts);
  }
}

}  // namespace axp2101
}  // namespace esphome

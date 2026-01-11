#include "axp2101.h"

#include "esphome/core/log.h"

namespace esphome {
namespace axp2101 {

static const char *const TAG = "axp2101";

// AXP2101 rail enable register mapping (enable bit = 1)
// NOTE: Register addresses are based on common AXP2101 documentation.
// If your board/IC revision differs, adjust accordingly.
static const AXP2101Component::RailEnableReg RAIL_ENS[] = {
    // DCDC
    {0x10, 1 << 2},  // dc3 -> DCDC_ENABLE register bit2
    // ALDOs
    {0x12, 1 << 0},  // aldo1
    {0x12, 1 << 1},  // aldo2
    {0x12, 1 << 2},  // aldo3
    {0x12, 1 << 3},  // aldo4
    // BLDOs
    {0x13, 1 << 0},  // bldo1
};

static const char *const RAIL_NAMES[] = {
    "dc3",
    "aldo1",
    "aldo2",
    "aldo3",
    "aldo4",
    "bldo1",
};

void AXP2101Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up AXP2101...");
}

void AXP2101Component::dump_config() {
  ESP_LOGCONFIG(TAG, "AXP2101:");
  LOG_I2C_DEVICE(this);
}

const AXP2101Component::RailEnableReg *AXP2101Component::find_rail_enable_reg_(const std::string &rail) const {
  for (size_t i = 0; i < (sizeof(RAIL_NAMES) / sizeof(RAIL_NAMES[0])); i++) {
    if (rail == RAIL_NAMES[i])
      return &RAIL_ENS[i];
  }
  return nullptr;
}

bool AXP2101Component::read_reg_(uint8_t reg, uint8_t &value) {
  if (!this->read_byte(reg, &value)) {
    ESP_LOGW(TAG, "I2C read failed reg 0x%02X", reg);
    return false;
  }
  this->reg_cache_[reg] = value;
  return true;
}

bool AXP2101Component::write_reg_(uint8_t reg, uint8_t value) {
  if (!this->write_byte(reg, value)) {
    ESP_LOGW(TAG, "I2C write failed reg 0x%02X", reg);
    return false;
  }
  this->reg_cache_[reg] = value;
  return true;
}

bool AXP2101Component::update_reg_(uint8_t reg, uint8_t mask, bool enabled) {
  uint8_t v;
  if (!this->read_reg_(reg, v))
    return false;

  if (enabled)
    v |= mask;
  else
    v &= ~mask;

  return this->write_reg_(reg, v);
}

bool AXP2101Component::get_rail_enabled(const std::string &rail, bool &enabled) {
  const auto *re = this->find_rail_enable_reg_(rail);
  if (re == nullptr) {
    ESP_LOGW(TAG, "Unknown rail '%s'", rail.c_str());
    return false;
  }

  uint8_t v;
  if (!this->read_reg_(re->reg, v))
    return false;

  enabled = (v & re->mask) != 0;
  return true;
}

bool AXP2101Component::set_rail_enabled(const std::string &rail, bool enabled, bool *read_back) {
  const auto *re = this->find_rail_enable_reg_(rail);
  if (re == nullptr) {
    ESP_LOGW(TAG, "Unknown rail '%s'", rail.c_str());
    return false;
  }

  if (!this->update_reg_(re->reg, re->mask, enabled))
    return false;

  if (read_back != nullptr) {
    bool rb{};
    if (!this->get_rail_enabled(rail, rb))
      return false;
    *read_back = rb;
  }

  return true;
}

}  // namespace axp2101
}  // namespace esphome

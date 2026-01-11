#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"

#include <map>

namespace esphome {
namespace axp2101 {

class AXP2101Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;

  // Runtime rail enable helpers with read-back
  // rail: one of dc3, aldo1..4, bldo1
  bool get_rail_enabled(const std::string &rail, bool &enabled);
  // If read_back != nullptr, updated with the state read from the chip after write.
  bool set_rail_enabled(const std::string &rail, bool enabled, bool *read_back = nullptr);

 protected:
  struct RailEnableReg {
    uint8_t reg;
    uint8_t mask;
  };

  const RailEnableReg *find_rail_enable_reg_(const std::string &rail) const;

  bool read_reg_(uint8_t reg, uint8_t &value);
  bool write_reg_(uint8_t reg, uint8_t value);
  bool update_reg_(uint8_t reg, uint8_t mask, bool enabled);

  // Cache of last read registers (optional future use)
  std::map<uint8_t, uint8_t> reg_cache_;
};

}  // namespace axp2101
}  // namespace esphome

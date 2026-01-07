#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace axp2101 {

class AXP2101Component : public Component {
 public:
  void setup() override;
  void dump_config() override;

  void set_i2c_address(uint8_t address) { this->i2c_address_ = address; }
  void set_debug_dump(bool debug_dump) { this->debug_dump_ = debug_dump; }

 protected:
  uint8_t i2c_address_{0x34};
  bool debug_dump_{false};

  bool read_reg_(uint8_t reg, uint8_t *value);
  bool read_regs_(uint8_t reg, uint8_t *data, size_t len);
  bool write_reg_(uint8_t reg, uint8_t value);

  void debug_dump_registers_();
};

}  // namespace axp2101
}  // namespace esphome

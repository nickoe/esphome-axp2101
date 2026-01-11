#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace axp2101 {

class AXP2101Component;

class AXP2101RailSwitch : public switch_::Switch, public Component {
 public:
  void set_parent(AXP2101Component *parent) { parent_ = parent; }
  void set_rail(const std::string &rail) { rail_ = rail; }

  void setup() override;
  void dump_config() override;

 protected:
  void write_state(bool state) override;

  AXP2101Component *parent_{nullptr};
  std::string rail_{};
};

}  // namespace axp2101
}  // namespace esphome

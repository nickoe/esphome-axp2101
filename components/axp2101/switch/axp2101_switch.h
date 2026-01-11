#pragma once

#include "esphome/components/switch/switch.h"
#include "esphome/core/component.h"

#include "../axp2101.h"

namespace esphome {
namespace axp2101 {
namespace switch_ {

enum class AXP2101Rail {
  DC3,
  ALDO1,
  ALDO2,
  ALDO3,
  ALDO4,
  BLDO1,
};

class AXP2101RailSwitch : public switch_::Switch, public Component {
 public:
  void set_parent(AXP2101Component *parent) { this->parent_ = parent; }
  void set_rail(const std::string &rail);

  void setup() override;
  void dump_config() override;

 protected:
  void write_state(bool state) override;

  AXP2101Component *parent_{nullptr};
  AXP2101Rail rail_{AXP2101Rail::DC3};
};

}  // namespace switch_
}  // namespace axp2101
}  // namespace esphome

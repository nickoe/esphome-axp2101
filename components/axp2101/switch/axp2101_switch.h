#pragma once

#include "esphome/components/switch/switch.h"
#include "esphome/core/component.h"

#include "../axp2101.h"

namespace esphome {
namespace axp2101 {

class AXP2101RailSwitch : public switch_::Switch, public Component {
 public:
  void set_parent(AXP2101Component *parent) { this->parent_ = parent; }
  void set_rail(AXP2101Rail rail) { this->rail_ = rail; }

  void setup() override {
    // publish initial state from read-back
    if (this->parent_ != nullptr) {
      bool en = this->parent_->get_rail_enabled(this->rail_);
      this->publish_state(en);
    }
  }

  void update_readback() {
    if (this->parent_ == nullptr)
      return;
    bool en = this->parent_->get_rail_enabled(this->rail_);
    this->publish_state(en);
  }

 protected:
  void write_state(bool state) override {
    if (this->parent_ == nullptr) {
      this->publish_state(state);
      return;
    }
    // attempt set then publish read-back
    this->parent_->set_rail_enabled(this->rail_, state);
    bool en = this->parent_->get_rail_enabled(this->rail_);
    this->publish_state(en);
  }

  AXP2101Component *parent_{nullptr};
  AXP2101Rail rail_{AXP2101Rail::DC3};
};

}  // namespace axp2101
}  // namespace esphome

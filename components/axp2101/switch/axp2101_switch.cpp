#include "axp2101_switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace axp2101 {
namespace switch_ {

static const char *const TAG = "axp2101.switch";

void AXP2101RailSwitch::setup() {
  if (this->parent_ == nullptr) {
    ESP_LOGE(TAG, "No parent AXP2101 set");
    return;
  }
  this->publish_state(this->parent_->get_rail_enabled(this->rail_));
}

void AXP2101RailSwitch::dump_config() {
  ESP_LOGCONFIG(TAG, "AXP2101 Rail Switch");
}

void AXP2101RailSwitch::write_state(bool state) {
  if (this->parent_ == nullptr) {
    return;
  }
  bool ok = this->parent_->set_rail_enabled(this->rail_, state);
  if (!ok) {
    ESP_LOGW(TAG, "Failed to set rail %d enabled=%s", (int) this->rail_, state ? "true" : "false");
  }
  // Re-read to ensure we publish actual state
  this->publish_state(this->parent_->get_rail_enabled(this->rail_));
}

}  // namespace switch_
}  // namespace axp2101
}  // namespace esphome

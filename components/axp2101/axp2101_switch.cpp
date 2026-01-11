#include "axp2101_switch.h"
#include "axp2101.h"

#include "esphome/core/log.h"

namespace esphome {
namespace axp2101 {

static const char *const TAG = "axp2101.switch";

void AXP2101RailSwitch::setup() {
  if (this->parent_ == nullptr) {
    ESP_LOGE(TAG, "No parent set on rail switch");
    return;
  }

  // Initialize state from chip read-back
  bool enabled = false;
  bool ok = this->parent_->get_rail_enabled(this->rail_, enabled);
  if (ok) {
    this->publish_state(enabled);
  } else {
    ESP_LOGW(TAG, "Failed to read rail state for '%s'", this->rail_.c_str());
  }
}

void AXP2101RailSwitch::dump_config() {
  LOG_SWITCH("AXP2101 Rail Switch", "", this);
  ESP_LOGCONFIG(TAG, "  Rail: %s", this->rail_.c_str());
}

void AXP2101RailSwitch::write_state(bool state) {
  if (this->parent_ == nullptr) {
    ESP_LOGE(TAG, "No parent set on rail switch");
    return;
  }

  bool rb = state;
  bool ok = this->parent_->set_rail_enabled(this->rail_, state, &rb);
  if (!ok) {
    ESP_LOGW(TAG, "Failed setting rail '%s' to %s", this->rail_.c_str(), ONOFF(state));
    // Keep optimistic? Here we keep last known.
    return;
  }

  this->publish_state(rb);
}

}  // namespace axp2101
}  // namespace esphome

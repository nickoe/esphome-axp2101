#include "axp2101.h"
#include "esphome/core/log.h"

namespace esphome {
namespace axp2101 {

static const char *const TAG = "axp2101";

void AXP2101Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up AXP2101...");
}

void AXP2101Component::dump_config() {
  ESP_LOGCONFIG(TAG, "AXP2101:");
}

}  // namespace axp2101
}  // namespace esphome

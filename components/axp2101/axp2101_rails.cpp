#include "axp2101.h"

namespace esphome {
namespace axp2101 {

bool AXP2101Component::get_rail_enabled(switch_::AXP2101Rail rail) {
  // TODO: implement by reading AXP2101 registers
  (void) rail;
  return false;
}

bool AXP2101Component::set_rail_enabled(switch_::AXP2101Rail rail, bool enabled) {
  // TODO: implement by writing AXP2101 registers
  (void) rail;
  (void) enabled;
  return false;
}

}  // namespace axp2101
}  // namespace esphome

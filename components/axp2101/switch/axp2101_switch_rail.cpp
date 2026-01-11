#include "axp2101_switch.h"

namespace esphome {
namespace axp2101 {
namespace switch_ {

void AXP2101RailSwitch::set_rail(const std::string &rail) {
  if (rail == "dc3")
    this->rail_ = AXP2101Rail::DC3;
  else if (rail == "aldo1")
    this->rail_ = AXP2101Rail::ALDO1;
  else if (rail == "aldo2")
    this->rail_ = AXP2101Rail::ALDO2;
  else if (rail == "aldo3")
    this->rail_ = AXP2101Rail::ALDO3;
  else if (rail == "aldo4")
    this->rail_ = AXP2101Rail::ALDO4;
  else if (rail == "bldo1")
    this->rail_ = AXP2101Rail::BLDO1;
}

}  // namespace switch_
}  // namespace axp2101
}  // namespace esphome

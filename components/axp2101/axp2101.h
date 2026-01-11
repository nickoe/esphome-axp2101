#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace axp2101 {
namespace switch_ {
  enum class AXP2101Rail;
}

class AXP2101Component : public Component {
 public:
  void setup() override;
  void dump_config() override;

  bool get_rail_enabled(switch_::AXP2101Rail rail);
  bool set_rail_enabled(switch_::AXP2101Rail rail, bool enabled);
};

}  // namespace axp2101
}  // namespace esphome

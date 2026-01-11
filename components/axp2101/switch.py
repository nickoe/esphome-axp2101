from __future__ import annotations

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch

from esphome.const import CONF_ID

from .component import axp2101_ns, AXP2101Component

CONF_AXP2101_ID = "axp2101_id"
CONF_RAIL = "rail"

axp2101_switch_ns = axp2101_ns.namespace("switch")
AXP2101RailSwitch = axp2101_switch_ns.class_("AXP2101RailSwitch", switch.Switch, cg.Component)


RAIL_OPTIONS = {
    "dc3": "DC3",
    "aldo1": "ALDO1",
    "aldo2": "ALDO2",
    "aldo3": "ALDO3",
    "aldo4": "ALDO4",
    "bldo1": "BLDO1",
}


CONFIG_SCHEMA = switch.switch_schema(AXP2101RailSwitch).extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(AXP2101RailSwitch),
        cv.Required(CONF_AXP2101_ID): cv.use_id(AXP2101Component),
        cv.Required(CONF_RAIL): cv.one_of(*RAIL_OPTIONS, lower=True),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await switch.register_switch(var, config)

    parent = await cg.get_variable(config[CONF_AXP2101_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_rail(config[CONF_RAIL]))

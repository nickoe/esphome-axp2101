import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID

from . import axp2101_ns, AXP2101Component, CONF_AXP2101_ID

DEPENDENCIES = ["axp2101"]

AXP2101RailSwitch = axp2101_ns.class_("AXP2101RailSwitch", switch.Switch, cg.Component)

CONF_RAIL = "rail"

RAILS = {
    "dc3": "dc3",
    "aldo1": "aldo1",
    "aldo2": "aldo2",
    "aldo3": "aldo3",
    "aldo4": "aldo4",
    "bldo1": "bldo1",
}

CONFIG_SCHEMA = switch.switch_schema(AXP2101RailSwitch).extend(
    {
        cv.GenerateID(CONF_AXP2101_ID): cv.use_id(AXP2101Component),
        cv.Required(CONF_RAIL): cv.one_of(*RAILS.keys(), lower=True),
    }
)


async def to_code(config):
    var = await switch.new_switch(config)
    await cg.register_component(var, config)

    parent = await cg.get_variable(config[CONF_AXP2101_ID])
    cg.add(var.set_parent(parent))

    cg.add(var.set_rail(RAILS[config[CONF_RAIL]]))

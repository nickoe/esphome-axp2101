import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID

from .. import axp2101_ns, AXP2101Component, CONF_AXP2101_ID

CONF_RAIL = "rail"

AXP2101RailSwitch = axp2101_ns.class_("AXP2101RailSwitch", switch.Switch, cg.Component)

RAILS = {
    "dc3": "DC3",
    "aldo1": "ALDO1",
    "aldo2": "ALDO2",
    "aldo3": "ALDO3",
    "aldo4": "ALDO4",
    "bldo1": "BLDO1",
}

CONFIG_SCHEMA = switch.switch_schema(AXP2101RailSwitch).extend(
    {
        cv.GenerateID(CONF_AXP2101_ID): cv.use_id(AXP2101Component),
        cv.Required(CONF_RAIL): cv.one_of(*RAILS.keys(), lower=True),
    }
)

async def to_code(config):
    var = await switch.new_switch(config)
    par = await cg.get_variable(config[CONF_AXP2101_ID])
    cg.add(var.set_parent(par))

    rail = RAILS[config[CONF_RAIL]]
    cg.add(var.set_rail(getattr(cg.global_ns.enum("AXP2101Rail"), rail)))

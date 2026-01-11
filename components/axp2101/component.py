from __future__ import annotations

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

axp2101_ns = cg.esphome_ns.namespace("axp2101")
AXP2101Component = axp2101_ns.class_("AXP2101Component", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(AXP2101Component),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

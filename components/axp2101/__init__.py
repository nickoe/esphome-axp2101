import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ADDRESS

CODEOWNERS = ["@nickoe"]

axp2101_ns = cg.esphome_ns.namespace("axp2101")
AXP2101Component = axp2101_ns.class_("AXP2101Component", cg.Component)

CONF_DEBUG_DUMP = "debug_dump"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(AXP2101Component),
        cv.Optional(CONF_ADDRESS, default=0x34): cv.hex_uint8_t,
        cv.Optional(CONF_DEBUG_DUMP, default=False): cv.boolean,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[cg.CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_i2c_address(config[CONF_ADDRESS]))
    cg.add(var.set_debug_dump(config[CONF_DEBUG_DUMP]))

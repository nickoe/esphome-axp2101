import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

CODEOWNERS = ["@nickoe"]

axp2101_ns = cg.esphome_ns.namespace("axp2101")
AXP2101Component = axp2101_ns.class_("AXP2101Component", cg.Component, i2c.I2CDevice)

# YAML keys
CONF_DC3 = "dc3"
CONF_ALDO1 = "aldo1"
CONF_ALDO2 = "aldo2"
CONF_ALDO3 = "aldo3"
CONF_ALDO4 = "aldo4"
CONF_BLDO1 = "bldo1"

CONF_ENABLE = "enable"
CONF_VOLTAGE = "voltage"


def _rail_schema(required: bool = True):
    # Voltage in mV. We keep it permissive and validate driver-side too.
    base = {
        cv.Optional(CONF_ENABLE, default=False): cv.boolean,
        cv.Optional(CONF_VOLTAGE): cv.int_range(min=0, max=5000),
    }
    if required:
        return cv.Schema(base)
    return cv.Schema(base)


CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(AXP2101Component),
            # Required rails
            cv.Required(CONF_DC3): _rail_schema(required=True),
            cv.Required(CONF_ALDO1): _rail_schema(required=True),
            cv.Required(CONF_ALDO2): _rail_schema(required=True),
            cv.Required(CONF_ALDO4): _rail_schema(required=True),
            # Optional rails
            cv.Optional(CONF_ALDO3): _rail_schema(required=False),
            cv.Optional(CONF_BLDO1): _rail_schema(required=False),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x34))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    def apply_rail(name: str, rail_conf):
        cg.add(getattr(var, f"set_{name}_enable")(rail_conf[CONF_ENABLE]))
        if CONF_VOLTAGE in rail_conf:
            cg.add(getattr(var, f"set_{name}_voltage_mv")(rail_conf[CONF_VOLTAGE]))

    apply_rail("dc3", config[CONF_DC3])
    apply_rail("aldo1", config[CONF_ALDO1])
    apply_rail("aldo2", config[CONF_ALDO2])
    if CONF_ALDO3 in config:
        apply_rail("aldo3", config[CONF_ALDO3])
    apply_rail("aldo4", config[CONF_ALDO4])
    if CONF_BLDO1 in config:
        apply_rail("bldo1", config[CONF_BLDO1])

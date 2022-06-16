import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, ble_client
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_VOLTAGE,
    ENTITY_CATEGORY_DIAGNOSTIC,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_VOLT,
)

CODEOWNERS = ["@kroimon"]

CONF_CHANNEL1_VOLTAGE = "channel1_voltage"
CONF_CHANNEL1_CURRENT = "channel1_current"
CONF_CHANNEL1_CAPACITY = "channel1_capacity"
CONF_CHANNEL1_TEMPERATURE = "channel1_temperature"
CONF_CHANNEL2_VOLTAGE = "channel2_voltage"
CONF_CHANNEL2_CURRENT = "channel2_current"
CONF_CHANNEL2_CAPACITY = "channel2_capacity"
CONF_CHANNEL2_TEMPERATURE = "channel2_temperature"
CONF_CHANNEL3_VOLTAGE = "channel3_voltage"
CONF_CHANNEL3_CURRENT = "channel3_current"
CONF_CHANNEL3_CAPACITY = "channel3_capacity"
CONF_CHANNEL3_TEMPERATURE = "channel3_temperature"
CONF_CHANNEL4_VOLTAGE = "channel4_voltage"
CONF_CHANNEL4_CURRENT = "channel4_current"
CONF_CHANNEL4_CAPACITY = "channel4_capacity"
CONF_CHANNEL4_TEMPERATURE = "channel4_temperature"

UNIT_MILLIAMPERE_HOUR = "mAh"

mc3000_ns = cg.esphome_ns.namespace("mc3000")
Mc3000 = mc3000_ns.class_("Mc3000", ble_client.BLEClientNode, cg.PollingComponent)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Mc3000),
            cv.Optional(CONF_CHANNEL1_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                device_class=DEVICE_CLASS_VOLTAGE,
                accuracy_decimals=2,
            ),
            cv.Optional(CONF_CHANNEL1_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                accuracy_decimals=2,
            ),
            cv.Optional(CONF_CHANNEL1_CAPACITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIAMPERE_HOUR,
                accuracy_decimals=0,
            ),
            cv.Optional(CONF_CHANNEL1_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=0,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_CHANNEL2_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                device_class=DEVICE_CLASS_VOLTAGE,
                accuracy_decimals=2,
            ),
            cv.Optional(CONF_CHANNEL2_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                accuracy_decimals=2,
            ),
            cv.Optional(CONF_CHANNEL2_CAPACITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIAMPERE_HOUR,
                accuracy_decimals=0,
            ),
            cv.Optional(CONF_CHANNEL2_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=0,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_CHANNEL3_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                device_class=DEVICE_CLASS_VOLTAGE,
                accuracy_decimals=2,
            ),
            cv.Optional(CONF_CHANNEL3_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                accuracy_decimals=2,
            ),
            cv.Optional(CONF_CHANNEL3_CAPACITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIAMPERE_HOUR,
                accuracy_decimals=0,
            ),
            cv.Optional(CONF_CHANNEL3_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=0,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_CHANNEL4_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                device_class=DEVICE_CLASS_VOLTAGE,
                accuracy_decimals=2,
            ),
            cv.Optional(CONF_CHANNEL4_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                accuracy_decimals=2,
            ),
            cv.Optional(CONF_CHANNEL4_CAPACITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIAMPERE_HOUR,
                accuracy_decimals=0,
            ),
            cv.Optional(CONF_CHANNEL4_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=0,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
        }
    )
    .extend(ble_client.BLE_CLIENT_SCHEMA)
    .extend(cv.polling_component_schema("5s"))
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield ble_client.register_ble_node(var, config)

    if CONF_CHANNEL1_VOLTAGE in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL1_VOLTAGE])
        cg.add(var.set_channel_voltage(0, sens))
    if CONF_CHANNEL1_CURRENT in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL1_CURRENT])
        cg.add(var.set_channel_current(0, sens))
    if CONF_CHANNEL1_CAPACITY in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL1_CAPACITY])
        cg.add(var.set_channel_capacity(0, sens))
    if CONF_CHANNEL1_TEMPERATURE in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL1_TEMPERATURE])
        cg.add(var.set_channel_temperature(0, sens))

    if CONF_CHANNEL2_VOLTAGE in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL2_VOLTAGE])
        cg.add(var.set_channel_voltage(1, sens))
    if CONF_CHANNEL2_CURRENT in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL2_CURRENT])
        cg.add(var.set_channel_current(1, sens))
    if CONF_CHANNEL2_CAPACITY in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL2_CAPACITY])
        cg.add(var.set_channel_capacity(1, sens))
    if CONF_CHANNEL2_TEMPERATURE in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL2_TEMPERATURE])
        cg.add(var.set_channel_temperature(1, sens))

    if CONF_CHANNEL3_VOLTAGE in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL3_VOLTAGE])
        cg.add(var.set_channel_voltage(2, sens))
    if CONF_CHANNEL3_CURRENT in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL3_CURRENT])
        cg.add(var.set_channel_current(2, sens))
    if CONF_CHANNEL3_CAPACITY in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL3_CAPACITY])
        cg.add(var.set_channel_capacity(2, sens))
    if CONF_CHANNEL3_TEMPERATURE in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL3_TEMPERATURE])
        cg.add(var.set_channel_temperature(2, sens))

    if CONF_CHANNEL4_VOLTAGE in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL4_VOLTAGE])
        cg.add(var.set_channel_voltage(3, sens))
    if CONF_CHANNEL4_CURRENT in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL4_CURRENT])
        cg.add(var.set_channel_current(3, sens))
    if CONF_CHANNEL4_CAPACITY in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL4_CAPACITY])
        cg.add(var.set_channel_capacity(3, sens))
    if CONF_CHANNEL4_TEMPERATURE in config:
        sens = yield sensor.new_sensor(config[CONF_CHANNEL4_TEMPERATURE])
        cg.add(var.set_channel_temperature(3, sens))

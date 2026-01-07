void AXP2101Component::dump_config() {
  ESP_LOGCONFIG(TAG, "AXP2101 Configuration:");
  ESP_LOGCONFIG(TAG, "  I2C Address: 0x%02X", this->i2c_address_);

  auto log_rail = [](const char *name, bool enabled, uint16_t voltage_mv, bool voltage_set) {
    if (!enabled) {
      ESP_LOGCONFIG(TAG, "  %s: disabled (not touched)", name);
    } else if (voltage_set) {
      ESP_LOGCONFIG(TAG, "  %s: enabled, set to %.2f V", name, voltage_mv / 1000.0f);
    } else {
      ESP_LOGCONFIG(TAG, "  %s: enabled, voltage unchanged", name);
    }
  };

  log_rail("DC3", this->dc3_enable_, this->dc3_voltage_mv_, this->dc3_voltage_set_);
  log_rail("ALDO1", this->aldo1_enable_, this->aldo1_voltage_mv_, this->aldo1_voltage_set_);
  log_rail("ALDO2", this->aldo2_enable_, this->aldo2_voltage_mv_, this->aldo2_voltage_set_);
  log_rail("ALDO3", this->aldo3_enable_, this->aldo3_voltage_mv_, this->aldo3_voltage_set_);
  log_rail("ALDO4", this->aldo4_enable_, this->aldo4_voltage_mv_, this->aldo4_voltage_set_);
  log_rail("BLDO1", this->bldo1_enable_, this->bldo1_voltage_mv_, this->bldo1_voltage_set_);
}
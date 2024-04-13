  void UpdatePowerStatus(const PowerSupplyStatus& status) {
    supply_status_ = status;
    if (supply_status_.battery_is_full)
      supply_status_.battery_percentage = 100.0;

    UpdateText();
  }

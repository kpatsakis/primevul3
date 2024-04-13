  void UpdateText() {
    base::TimeDelta time = base::TimeDelta::FromSeconds(
        supply_status_.line_power_on ?
        supply_status_.battery_seconds_to_full :
        supply_status_.battery_seconds_to_empty);
    int hour = time.InHours();
    int min = (time - base::TimeDelta::FromHours(hour)).InMinutes();
    ui::ResourceBundle& bundle = ui::ResourceBundle::GetSharedInstance();
    if (hour || min) {
      SetText(l10n_util::GetStringFUTF16(IDS_ASH_STATUS_TRAY_BATTERY_STATUS,
          base::IntToString16(
              static_cast<int>(supply_status_.battery_percentage)),
          base::IntToString16(hour),
          base::IntToString16(min)));
    } else {
      if (supply_status_.line_power_on) {
        SetText(bundle.GetLocalizedString(IDS_ASH_STATUS_TRAY_BATTERY_FULL));
      } else {
        SetText(string16());
      }
    }
  }

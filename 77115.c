void TrayPower::OnPowerStatusChanged(const PowerSupplyStatus& status) {
  if (power_tray_.get())
    power_tray_->UpdatePowerStatus(status);
  if (power_.get())
    power_->UpdatePowerStatus(status);
}

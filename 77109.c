views::View* TrayPower::CreateDefaultView(user::LoginStatus status) {
  date_.reset(new tray::DateView(tray::DateView::DATE));
  if (status != user::LOGGED_IN_NONE && status != user::LOGGED_IN_LOCKED)
    date_->set_actionable(true);

  views::View* container = new views::View;
  views::BoxLayout* layout = new views::BoxLayout(views::BoxLayout::kHorizontal,
      kTrayPopupPaddingHorizontal, 10, 0);
  layout->set_spread_blank_space(true);
  container->SetLayoutManager(layout);
  container->set_background(views::Background::CreateSolidBackground(
      SkColorSetRGB(0xf1, 0xf1, 0xf1)));
  container->AddChildView(date_.get());

  PowerSupplyStatus power_status =
      ash::Shell::GetInstance()->tray_delegate()->GetPowerSupplyStatus();
  if (power_status.battery_is_present) {
    power_.reset(new tray::PowerPopupView());
    power_->UpdatePowerStatus(power_status);
    container->AddChildView(power_.get());
  }
  return container;
}

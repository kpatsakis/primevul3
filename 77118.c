  void UpdateImage() {
    SkBitmap image;
    gfx::Image all = ui::ResourceBundle::GetSharedInstance().GetImageNamed(
        IDR_AURA_UBER_TRAY_POWER_SMALL);

    int image_index = 0;
    if (supply_status_.battery_percentage >= 100) {
      image_index = kNumPowerImages - 1;
    } else if (!supply_status_.battery_is_present) {
      image_index = kNumPowerImages;
    } else {
      image_index = static_cast<int> (
          supply_status_.battery_percentage / 100.0 *
          (kNumPowerImages - 1));
      image_index =
        std::max(std::min(image_index, kNumPowerImages - 2), 0);
    }

    SkIRect region = SkIRect::MakeXYWH(
        (supply_status_.line_power_on ? kBatteryImageWidth : 0) + 2,
        image_index * kBatteryImageHeight,
        kBatteryImageWidth - 2, kBatteryImageHeight);
    all.ToSkBitmap()->extractSubset(&image, region);

    SetImage(image);
  }

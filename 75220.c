void BrowserTitlebar::UpdateTextColor() {
  if (app_mode_title_) {
    if (theme_provider_ && theme_provider_->UseGtkTheme()) {
      GdkColor frame_color;
      if (window_has_focus_) {
        frame_color = theme_provider_->GetGdkColor(
          BrowserThemeProvider::COLOR_FRAME);
      } else {
        frame_color = theme_provider_->GetGdkColor(
          BrowserThemeProvider::COLOR_FRAME_INACTIVE);
      }
      GdkColor text_color = PickLuminosityContrastingColor(
          &frame_color, &gfx::kGdkWhite, &gfx::kGdkBlack);
      gtk_util::SetLabelColor(app_mode_title_, &text_color);
    } else {
      gtk_util::SetLabelColor(app_mode_title_, &gfx::kGdkWhite);
    }
  }
}

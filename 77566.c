void BrowserNonClientFrameViewAura::Init() {
  maximize_button_ = new ash::FrameMaximizeButton(this, this);
  maximize_button_->SetAccessibleName(
      l10n_util::GetStringUTF16(IDS_ACCNAME_MAXIMIZE));
  AddChildView(maximize_button_);
  close_button_ = new views::ImageButton(this);
  close_button_->SetAccessibleName(
      l10n_util::GetStringUTF16(IDS_ACCNAME_CLOSE));
  AddChildView(close_button_);

  if (browser_view()->ShouldShowWindowIcon()) {
    window_icon_ = new TabIconView(this);
    window_icon_->set_is_light(true);
    AddChildView(window_icon_);
    window_icon_->Update();
  }

  UpdateAvatarInfo();

  frame_painter_->Init(frame(), window_icon_, maximize_button_, close_button_);
}

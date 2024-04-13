void WebContentsImpl::OnOpenColorChooser(int color_chooser_id,
                                         SkColor color) {
  ColorChooser* new_color_chooser = delegate_->OpenColorChooser(this, color);
  if (color_chooser_ == new_color_chooser)
    return;
  color_chooser_.reset(new_color_chooser);
  color_chooser_identifier_ = color_chooser_id;
}

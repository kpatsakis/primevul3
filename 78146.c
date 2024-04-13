void WebContentsImpl::OnSetSelectedColorInColorChooser(int color_chooser_id,
                                                       SkColor color) {
  if (color_chooser_ &&
      color_chooser_id == color_chooser_identifier_)
    color_chooser_->SetSelectedColor(color);
}

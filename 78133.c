void WebContentsImpl::OnEndColorChooser(int color_chooser_id) {
  if (color_chooser_ &&
      color_chooser_id == color_chooser_identifier_)
    color_chooser_->End();
}

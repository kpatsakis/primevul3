void ColorChooserWin::OnColorChooserDialogClosed() {
  if (color_chooser_dialog_.get()) {
    color_chooser_dialog_->ListenerDestroyed();
    color_chooser_dialog_ = NULL;
  }
  DCHECK(current_color_chooser_ == this);
  current_color_chooser_ = NULL;
  if (web_contents_)
    web_contents_->DidEndColorChooser();
}

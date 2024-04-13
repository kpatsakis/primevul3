void BrowserTitlebar::OnButtonClicked(GtkWidget* button,
                                      BrowserTitlebar* titlebar) {
  if (titlebar->close_button_->widget() == button) {
    titlebar->browser_window_->Close();
  } else if (titlebar->restore_button_->widget() == button) {
    titlebar->browser_window_->UnMaximize();
  } else if (titlebar->maximize_button_->widget() == button) {
    gtk_window_maximize(titlebar->window_);
  } else if (titlebar->minimize_button_->widget() == button) {
    gtk_window_iconify(titlebar->window_);
  }
}

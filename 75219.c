void BrowserTitlebar::UpdateCustomFrame(bool use_custom_frame) {
  using_custom_frame_ = use_custom_frame;
  if (use_custom_frame)
    gtk_widget_show(titlebar_buttons_box_);
  else
    gtk_widget_hide(titlebar_buttons_box_);
  UpdateTitlebarAlignment();
}

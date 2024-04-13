void BrowserTitlebar::UpdateTitleAndIcon() {
  if (!app_mode_title_)
    return;

  string16 title = browser_window_->browser()->GetWindowTitleForCurrentTab();
  gtk_label_set_text(GTK_LABEL(app_mode_title_), UTF16ToUTF8(title).c_str());

  if (browser_window_->browser()->type() == Browser::TYPE_APP) {
    SkBitmap icon = browser_window_->browser()->GetCurrentPageIcon();
    if (icon.empty()) {
      gtk_util::SetWindowIcon(window_);
    } else {
      GdkPixbuf* icon_pixbuf = gfx::GdkPixbufFromSkBitmap(&icon);
      gtk_window_set_icon(window_, icon_pixbuf);
      g_object_unref(icon_pixbuf);
    }
  }
}

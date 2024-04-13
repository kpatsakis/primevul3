void BrowserTitlebar::UpdateThrobber(TabContents* tab_contents) {
  DCHECK(app_mode_favicon_);

  if (tab_contents && tab_contents->is_loading()) {
    GdkPixbuf* icon_pixbuf =
        throbber_.GetNextFrame(tab_contents->waiting_for_response());
    gtk_image_set_from_pixbuf(GTK_IMAGE(app_mode_favicon_), icon_pixbuf);
  } else {
    ResourceBundle& rb = ResourceBundle::GetSharedInstance();
    if (browser_window_->browser()->type() == Browser::TYPE_APP) {
      SkBitmap icon = browser_window_->browser()->GetCurrentPageIcon();
      if (icon.empty()) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_mode_favicon_),
            rb.GetPixbufNamed(IDR_PRODUCT_LOGO_16));
      } else {
        GdkPixbuf* icon_pixbuf = gfx::GdkPixbufFromSkBitmap(&icon);
        gtk_image_set_from_pixbuf(GTK_IMAGE(app_mode_favicon_), icon_pixbuf);
        g_object_unref(icon_pixbuf);
      }
    } else {
      gtk_image_set_from_pixbuf(GTK_IMAGE(app_mode_favicon_),
          rb.GetPixbufNamed(IDR_PRODUCT_LOGO_16));
    }
    throbber_.Reset();
  }
}

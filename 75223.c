void BrowserTitlebar::UpdateTitlebarAlignment() {
  if (browser_window_->browser()->type() == Browser::TYPE_NORMAL) {
    if (using_custom_frame_ && !browser_window_->IsMaximized()) {
      gtk_alignment_set_padding(GTK_ALIGNMENT(titlebar_alignment_),
          kTitlebarHeight, 0, kTabStripLeftPadding, 0);
    } else {
      gtk_alignment_set_padding(GTK_ALIGNMENT(titlebar_alignment_), 0, 0,
                                kTabStripLeftPadding, 0);
    }
  } else {
    if (using_custom_frame_ && !browser_window_->IsFullscreen()) {
      gtk_alignment_set_padding(GTK_ALIGNMENT(titlebar_alignment_),
          kAppModePaddingTop, kAppModePaddingBottom, kAppModePaddingLeft, 0);
      gtk_widget_show(titlebar_alignment_);
    } else {
      gtk_widget_hide(titlebar_alignment_);
    }
  }

  GtkRequisition close_button_req = close_button_req_;
  GtkRequisition minimize_button_req = minimize_button_req_;
  GtkRequisition restore_button_req = restore_button_req_;
  if (using_custom_frame_ && browser_window_->IsMaximized()) {
    close_button_req.width += kButtonOuterPadding;
    close_button_req.height += kButtonOuterPadding;
    minimize_button_req.height += kButtonOuterPadding;
    restore_button_req.height += kButtonOuterPadding;
    gtk_widget_hide(top_padding_);
  } else {
    gtk_widget_show(top_padding_);
  }
  gtk_widget_set_size_request(close_button_->widget(),
                              close_button_req.width, close_button_req.height);
  gtk_widget_set_size_request(minimize_button_->widget(),
                              minimize_button_req.width,
                              minimize_button_req.height);
  gtk_widget_set_size_request(restore_button_->widget(),
                              restore_button_req.width,
                              restore_button_req.height);
}

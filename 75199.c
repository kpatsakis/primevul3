CustomDrawButton* BrowserTitlebar::BuildTitlebarButton(int image,
    int image_pressed, int image_hot, GtkWidget* box, int tooltip) {
  CustomDrawButton* button = new CustomDrawButton(image, image_pressed,
                                                  image_hot, 0);
  gtk_widget_add_events(GTK_WIDGET(button->widget()), GDK_POINTER_MOTION_MASK);
  g_signal_connect(button->widget(), "clicked", G_CALLBACK(OnButtonClicked),
                   this);
  g_signal_connect(button->widget(), "motion-notify-event",
                   G_CALLBACK(OnMouseMoveEvent), browser_window_);
  std::string localized_tooltip = l10n_util::GetStringUTF8(tooltip);
  gtk_widget_set_tooltip_text(button->widget(),
                              localized_tooltip.c_str());
  gtk_box_pack_end(GTK_BOX(box), button->widget(), FALSE, FALSE, 0);
  return button;
}

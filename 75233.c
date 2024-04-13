void MenuGtk::OnMenuHidden(GtkWidget* widget, MenuGtk* menu) {
  menu->delegate_->StoppedShowing();
}

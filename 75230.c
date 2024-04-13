void MenuGtk::ConnectSignalHandlers() {
  g_signal_connect(menu_.get(), "show", G_CALLBACK(OnMenuShow), this);
  g_signal_connect(menu_.get(), "hide", G_CALLBACK(OnMenuHidden), this);
}

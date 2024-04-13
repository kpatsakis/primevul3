MenuGtk::MenuGtk(MenuGtk::Delegate* delegate, bool load)
    : delegate_(delegate),
      dummy_accel_group_(NULL),
      menu_(gtk_menu_new()),
      factory_(this) {
  ConnectSignalHandlers();
  if (load)
    BuildMenuFromDelegate();
}

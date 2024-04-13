void RootWindowHostLinux::Show() {
  XSizeHints size_hints;
  size_hints.flags = PPosition;
  size_hints.x = bounds_.x();
  size_hints.y = bounds_.y();
  XSetWMNormalHints(xdisplay_, xwindow_, &size_hints);

  XMapWindow(xdisplay_, xwindow_);
}

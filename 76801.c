void RootWindowHostLinux::MoveCursorTo(const gfx::Point& location) {
  XWarpPointer(xdisplay_, None, xwindow_, 0, 0, 0, 0, location.x(),
      location.y());
}

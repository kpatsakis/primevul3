gfx::Point RootWindowHostLinux::QueryMouseLocation() {
  ::Window root_return, child_return;
  int root_x_return, root_y_return, win_x_return, win_y_return;
  unsigned int mask_return;
  XQueryPointer(xdisplay_,
                xwindow_,
                &root_return,
                &child_return,
                &root_x_return, &root_y_return,
                &win_x_return, &win_y_return,
                &mask_return);
  return gfx::Point(max(0, min(bounds_.width(), win_x_return)),
                    max(0, min(bounds_.height(), win_y_return)));
}

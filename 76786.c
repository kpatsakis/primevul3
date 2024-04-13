bool RootWindowHostLinux::ConfineCursorToRootWindow() {
#if XFIXES_MAJOR >= 5
  DCHECK(!pointer_barriers_.get());
  if (pointer_barriers_.get())
    return false;
  gfx::Size screen_size = RootWindowHost::GetNativeScreenSize();
  pointer_barriers_.reset(new XID[4]);
  pointer_barriers_[0] = XFixesCreatePointerBarrier(
      xdisplay_, x_root_window_,
      0, bounds_.y(), screen_size.width(), bounds_.y(),
      BarrierPositiveY,
      0, NULL);  // default device
  pointer_barriers_[1] = XFixesCreatePointerBarrier(
      xdisplay_, x_root_window_,
      0, bounds_.bottom(), screen_size.width(),  bounds_.bottom(),
      BarrierNegativeY,
      0, NULL);  // default device
  pointer_barriers_[2] = XFixesCreatePointerBarrier(
      xdisplay_, x_root_window_,
      bounds_.x(), 0, bounds_.x(), screen_size.height(),
      BarrierPositiveX,
      0, NULL);  // default device
  pointer_barriers_[3] = XFixesCreatePointerBarrier(
      xdisplay_, x_root_window_,
      bounds_.right(), 0, bounds_.right(), screen_size.height(),
      BarrierNegativeX,
      0, NULL);  // default device
#endif
  return true;
}

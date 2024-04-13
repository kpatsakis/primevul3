void RootWindowHostLinux::UnConfineCursor() {
#if XFIXES_MAJOR >= 5
  if (pointer_barriers_.get()) {
    XFixesDestroyPointerBarrier(xdisplay_, pointer_barriers_[0]);
    XFixesDestroyPointerBarrier(xdisplay_, pointer_barriers_[1]);
    XFixesDestroyPointerBarrier(xdisplay_, pointer_barriers_[2]);
    XFixesDestroyPointerBarrier(xdisplay_, pointer_barriers_[3]);
    pointer_barriers_.reset();
  }
#endif
}

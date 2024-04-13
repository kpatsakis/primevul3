scoped_refptr<CompositorLock> RootWindow::GetCompositorLock() {
  if (!compositor_lock_)
    compositor_lock_ = new CompositorLock(this);
  return compositor_lock_;
}

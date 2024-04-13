CompositorLock::CompositorLock(RootWindow* root_window)
    : root_window_(root_window) {
  MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&CompositorLock::CancelLock, AsWeakPtr()),
      kCompositorLockTimeoutMs);
}

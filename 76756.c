void RootWindow::OnRootWindowHostClosed() {
  FOR_EACH_OBSERVER(RootWindowObserver, observers_,
                    OnRootWindowHostClosed(this));
}

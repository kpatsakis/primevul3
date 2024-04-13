void RootWindow::OnKeyboardMappingChanged() {
  FOR_EACH_OBSERVER(RootWindowObserver, observers_,
                    OnKeyboardMappingChanged(this));
}

LRESULT RootWindowHostWin::OnCaptureChanged(UINT message,
                                            WPARAM w_param,
                                            LPARAM l_param) {
  if (has_capture_) {
    has_capture_ = false;
    root_window_->SetCapture(NULL);
  }
  return 0;
}

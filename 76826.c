LRESULT RootWindowHostWin::OnKeyEvent(UINT message,
                                      WPARAM w_param,
                                      LPARAM l_param) {
  MSG msg = { hwnd(), message, w_param, l_param };
  KeyEvent keyev(msg, message == WM_CHAR);
  SetMsgHandled(root_window_->DispatchKeyEvent(&keyev));
  return 0;
}

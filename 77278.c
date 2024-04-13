void OmniboxViewWin::HandleExternalMsg(UINT msg,
                                       UINT flags,
                                       const CPoint& screen_point) {
  if (msg == WM_CAPTURECHANGED) {
    SendMessage(msg, 0, NULL);
    return;
  }

  CPoint client_point(screen_point);
  ::MapWindowPoints(NULL, m_hWnd, &client_point, 1);
  SendMessage(msg, flags, MAKELPARAM(client_point.x, client_point.y));
}

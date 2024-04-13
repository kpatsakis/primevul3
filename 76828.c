void RootWindowHostWin::OnPaint(HDC dc) {
  root_window_->Draw();
  ValidateRect(hwnd(), NULL);
}

int OmniboxViewWin::GetHorizontalMargin() const {
  RECT rect;
  GetRect(&rect);
  RECT client_rect;
  GetClientRect(&client_rect);
  return (rect.left - client_rect.left) + (client_rect.right - rect.right);
}

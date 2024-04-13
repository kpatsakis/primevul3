int OmniboxViewWin::GetMaxEditWidth(int entry_width) const {
  RECT formatting_rect;
  GetRect(&formatting_rect);
  RECT edit_bounds;
  GetClientRect(&edit_bounds);
  return entry_width - formatting_rect.left -
      (edit_bounds.right - formatting_rect.right);
}

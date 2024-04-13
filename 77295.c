void OmniboxViewWin::OnContextMenu(HWND window, const CPoint& point) {
  BuildContextMenu();
  if (point.x == -1 || point.y == -1) {
    POINT p;
    GetCaretPos(&p);
    MapWindowPoints(HWND_DESKTOP, &p, 1);
    context_menu_->RunContextMenuAt(gfx::Point(p));
  } else {
    context_menu_->RunContextMenuAt(gfx::Point(point));
  }
}

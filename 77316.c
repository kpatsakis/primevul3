BOOL OmniboxViewWin::OnMouseWheel(UINT flags, short delta, CPoint point) {
  if (!ui::RerouteMouseWheel(m_hWnd, MAKEWPARAM(flags, delta),
                             MAKELPARAM(point.x, point.y)))
    SetMsgHandled(false);
  return 0;
}

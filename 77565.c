bool BrowserNonClientFrameViewAura::HitTest(const gfx::Point& l) const {
  if (NonClientFrameView::HitTest(l))
    return true;

  if (!browser_view()->tabstrip())
    return false;
  gfx::Rect tabstrip_bounds(browser_view()->tabstrip()->bounds());
  gfx::Point tabstrip_origin(tabstrip_bounds.origin());
  View::ConvertPointToView(frame()->client_view(), this, &tabstrip_origin);
  tabstrip_bounds.set_origin(tabstrip_origin);
  if (l.y() > tabstrip_bounds.bottom())
    return false;

  gfx::Point browser_view_point(l);
  View::ConvertPointToView(parent(), browser_view(), &browser_view_point);
  return browser_view()->IsPositionInWindowCaption(browser_view_point);
}

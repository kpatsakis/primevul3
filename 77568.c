 void BrowserNonClientFrameViewAura::LayoutAvatar() {
  DCHECK(avatar_button());
  SkBitmap incognito_icon = browser_view()->GetOTRAvatarIcon();

  int avatar_bottom = GetHorizontalTabStripVerticalOffset(false) +
      browser_view()->GetTabStripHeight() - kAvatarBottomSpacing;
  int avatar_restored_y = avatar_bottom - incognito_icon.height();
  int avatar_y = frame()->IsMaximized() ?
      NonClientTopBorderHeight(false) + kTabShadowHeight:
      avatar_restored_y;
  gfx::Rect avatar_bounds(kAvatarSideSpacing,
                          avatar_y,
                          incognito_icon.width(),
                          avatar_bottom - avatar_y);
  avatar_button()->SetBoundsRect(avatar_bounds);
}

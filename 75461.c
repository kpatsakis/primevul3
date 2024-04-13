void BrowserFrameGtk::TabStripDisplayModeChanged() {
  if (GetWidget()->GetRootView()->has_children()) {
    GetWidget()->GetRootView()->GetChildViewAt(0)->InvalidateLayout();
  }
  GetWidget()->GetRootView()->Layout();
}

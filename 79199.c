void DevToolsWindow::Restore() {
  if (dock_side_ == DEVTOOLS_DOCK_SIDE_MINIMIZED)
    SetDockSide(SideToString(dock_side_before_minimized_));
}

DevToolsDockSide DevToolsWindow::SideFromString(
    const std::string& dock_side) {
  if (dock_side == kDockSideRight)
    return DEVTOOLS_DOCK_SIDE_RIGHT;
  if (dock_side == kDockSideBottom)
    return DEVTOOLS_DOCK_SIDE_BOTTOM;
  return (dock_side == kDockSideMinimized) ?
      DEVTOOLS_DOCK_SIDE_MINIMIZED : DEVTOOLS_DOCK_SIDE_UNDOCKED;
}

std::string DevToolsWindow::SideToString(DevToolsDockSide dock_side) {
  switch (dock_side) {
    case DEVTOOLS_DOCK_SIDE_UNDOCKED:  return kDockSideUndocked;
    case DEVTOOLS_DOCK_SIDE_RIGHT:     return kDockSideRight;
    case DEVTOOLS_DOCK_SIDE_BOTTOM:    return kDockSideBottom;
    case DEVTOOLS_DOCK_SIDE_MINIMIZED: return kDockSideMinimized;
    default:                           return kDockSideUndocked;
  }
}

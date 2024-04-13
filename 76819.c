const wchar_t* GetCursorId(gfx::NativeCursor native_cursor) {
  switch (native_cursor.native_type()) {
    case ui::kCursorNull:
      return IDC_ARROW;
    case ui::kCursorPointer:
      return IDC_ARROW;
    case ui::kCursorCross:
      return IDC_CROSS;
    case ui::kCursorHand:
      return IDC_HAND;
    case ui::kCursorIBeam:
      return IDC_IBEAM;
    case ui::kCursorWait:
      return IDC_WAIT;
    case ui::kCursorHelp:
      return IDC_HELP;
    case ui::kCursorEastResize:
      return IDC_SIZEWE;
    case ui::kCursorNorthResize:
      return IDC_SIZENS;
    case ui::kCursorNorthEastResize:
      return IDC_SIZENESW;
    case ui::kCursorNorthWestResize:
      return IDC_SIZENWSE;
    case ui::kCursorSouthResize:
      return IDC_SIZENS;
    case ui::kCursorSouthEastResize:
      return IDC_SIZENWSE;
    case ui::kCursorSouthWestResize:
      return IDC_SIZENESW;
    case ui::kCursorWestResize:
      return IDC_SIZEWE;
    case ui::kCursorNorthSouthResize:
      return IDC_SIZENS;
    case ui::kCursorEastWestResize:
      return IDC_SIZEWE;
    case ui::kCursorNorthEastSouthWestResize:
      return IDC_SIZENESW;
    case ui::kCursorNorthWestSouthEastResize:
      return IDC_SIZENWSE;
    case ui::kCursorMove:
      return IDC_SIZEALL;
    case ui::kCursorProgress:
      return IDC_APPSTARTING;
    case ui::kCursorNoDrop:
      return IDC_NO;
    case ui::kCursorNotAllowed:
      return IDC_NO;
    case ui::kCursorColumnResize:
    case ui::kCursorRowResize:
    case ui::kCursorMiddlePanning:
    case ui::kCursorEastPanning:
    case ui::kCursorNorthPanning:
    case ui::kCursorNorthEastPanning:
    case ui::kCursorNorthWestPanning:
    case ui::kCursorSouthPanning:
    case ui::kCursorSouthEastPanning:
    case ui::kCursorSouthWestPanning:
    case ui::kCursorWestPanning:
    case ui::kCursorVerticalText:
    case ui::kCursorCell:
    case ui::kCursorContextMenu:
    case ui::kCursorAlias:
    case ui::kCursorCopy:
    case ui::kCursorNone:
    case ui::kCursorZoomIn:
    case ui::kCursorZoomOut:
    case ui::kCursorGrab:
    case ui::kCursorGrabbing:
    case ui::kCursorCustom:
      NOTIMPLEMENTED();
      return IDC_ARROW;
    default:
      NOTREACHED();
      return IDC_ARROW;
  }
}

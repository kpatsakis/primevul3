void RootWindowHostLinux::SetCursorInternal(gfx::NativeCursor cursor) {
  ::Cursor xcursor =
      image_cursors_->IsImageCursor(cursor) ?
      image_cursors_->ImageCursorFromNative(cursor) :
      cursor == ui::kCursorNone ?
      invisible_cursor_ :
      cursor == ui::kCursorCustom ?
      cursor.platform() :
      ui::GetXCursor(CursorShapeFromNative(cursor));
  XDefineCursor(xdisplay_, xwindow_, xcursor);
}

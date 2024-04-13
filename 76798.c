  bool IsImageCursor(gfx::NativeCursor native_cursor) {
    return cursors_.find(native_cursor.native_type()) != cursors_.end();
  }

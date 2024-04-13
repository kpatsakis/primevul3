  ::Cursor ImageCursorFromNative(gfx::NativeCursor native_cursor) {
    DCHECK(cursors_.find(native_cursor.native_type()) != cursors_.end());
    return cursors_[native_cursor.native_type()];
  }

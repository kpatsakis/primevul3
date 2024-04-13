BookmarkManagerView::CutCopyPasteType KeyCodeToCutCopyPaste(
    unsigned short virtual_keycode) {
  switch (virtual_keycode) {
    case VK_INSERT:
      if (GetKeyState(VK_CONTROL) < 0)
        return BookmarkManagerView::COPY;
      if (GetKeyState(VK_SHIFT) < 0)
        return BookmarkManagerView::PASTE;
      return BookmarkManagerView::NONE;

    case VK_DELETE:
      if (GetKeyState(VK_SHIFT) < 0)
        return BookmarkManagerView::CUT;
      return BookmarkManagerView::NONE;

    case 'C':
      if (GetKeyState(VK_CONTROL) < 0)
        return BookmarkManagerView::COPY;
      return BookmarkManagerView::NONE;

    case 'V':
      if (GetKeyState(VK_CONTROL) < 0)
        return BookmarkManagerView::PASTE;
      return BookmarkManagerView::NONE;

    case 'X':
      if (GetKeyState(VK_CONTROL) < 0)
        return BookmarkManagerView::CUT;
      return BookmarkManagerView::NONE;

    default:
      return BookmarkManagerView::NONE;
  }
}

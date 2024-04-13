int CALLBACK OmniboxViewWin::WordBreakProc(LPTSTR edit_text,
                                           int current_pos,
                                           int num_bytes,
                                           int action) {

  const int length = num_bytes / sizeof(wchar_t);

  switch (action) {
    case WB_LEFT:
    case WB_MOVEWORDLEFT: {
      if (current_pos < 2) {
        return 0;
      }

      const int prev_delim =
          WordBreakProc(edit_text, current_pos - 1, num_bytes, WB_LEFTBREAK);

      if ((prev_delim == 0) &&
          !WordBreakProc(edit_text, 0, num_bytes, WB_ISDELIMITER)) {
        return 0;
      }

      return prev_delim + 1;
    }

    case WB_RIGHT:
    case WB_MOVEWORDRIGHT: {
      if (WordBreakProc(edit_text, current_pos, num_bytes, WB_ISDELIMITER)) {
        return current_pos + 1;
      }

      const int next_delim =
          WordBreakProc(edit_text, current_pos, num_bytes, WB_RIGHTBREAK);
      if (next_delim == length) {
        return length;
      }

      return next_delim + 1;
    }

    case WB_ISDELIMITER:
      return !!(WordBreakProc(edit_text, current_pos, num_bytes, WB_CLASSIFY) &
                WBF_BREAKLINE);

    case WB_CLASSIFY:
      if (IsWhitespace(edit_text[current_pos])) {
        while ((current_pos < (length - 1)) &&
               (edit_text[current_pos] == 0x13)) {
          if (edit_text[++current_pos] == 0x10)
            return WBF_ISWHITE;
        }
        return WBF_BREAKLINE | WBF_ISWHITE;
      }

      if (ispunct(edit_text[current_pos], std::locale()) &&
          !SchemeEnd(edit_text, current_pos, length) &&
          !SchemeEnd(edit_text, current_pos - 1, length))
        return WBF_BREAKLINE;

      return 0;

    case WB_LEFTBREAK:
      for (int i = current_pos - 1; i >= 0; --i) {
        if (WordBreakProc(edit_text, i, num_bytes, WB_ISDELIMITER))
          return i;
      }
      return 0;

    case WB_RIGHTBREAK:
      for (int i = current_pos + 1; i < length; ++i) {
        if (WordBreakProc(edit_text, i, num_bytes, WB_ISDELIMITER))
          return i;
      }
      return length;
  }

  NOTREACHED();
  return 0;
}

bool IsIDNComponentInSingleScript(const char16* str, int str_len) {
  UScriptCode first_script = USCRIPT_INVALID_CODE;
  bool is_first = true;

  int i = 0;
  while (i < str_len) {
    unsigned code_point;
    U16_NEXT(str, i, str_len, code_point);

    UErrorCode err = U_ZERO_ERROR;
    UScriptCode cur_script = uscript_getScript(code_point, &err);
    if (err != U_ZERO_ERROR)
      return false;  // Report mixed on error.
    cur_script = NormalizeScript(cur_script);

    if (is_first && cur_script != USCRIPT_COMMON) {
      first_script = cur_script;
      is_first = false;
    } else {
      if (cur_script != USCRIPT_COMMON && cur_script != first_script)
        return false;
    }
  }
  return true;
}

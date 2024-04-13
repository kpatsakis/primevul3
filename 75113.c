std::string GetDirectoryListingEntry(const string16& name,
                                     const std::string& raw_bytes,
                                     bool is_dir,
                                     int64 size,
                                     Time modified) {
  std::string result;
  result.append("<script>addRow(");
  base::JsonDoubleQuote(name, true, &result);
  result.append(",");
  if (raw_bytes.empty()) {
    base::JsonDoubleQuote(EscapePath(UTF16ToUTF8(name)),
                                   true, &result);
  } else {
    base::JsonDoubleQuote(EscapePath(raw_bytes), true, &result);
  }
  if (is_dir) {
    result.append(",1,");
  } else {
    result.append(",0,");
  }

  base::JsonDoubleQuote(
      WideToUTF16Hack(FormatBytes(size, GetByteDisplayUnits(size), true)), true,
      &result);

  result.append(",");

  string16 modified_str;
  if (!modified.is_null()) {
    modified_str = WideToUTF16Hack(base::TimeFormatShortDateAndTime(modified));
  }
  base::JsonDoubleQuote(modified_str, true, &result);

  result.append(");</script>\n");

  return result;
}

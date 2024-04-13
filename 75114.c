std::string GetDirectoryListingHeader(const string16& title) {
  static const base::StringPiece header(
      NetModule::GetResource(IDR_DIR_HEADER_HTML));
  DLOG_IF(WARNING, header.empty()) <<
      "Missing resource: directory listing header";

  std::string result;
  if (!header.empty())
    result.assign(header.data(), header.size());

  result.append("<script>start(");
  base::JsonDoubleQuote(title, true, &result);
  result.append(");</script>\n");

  return result;
}

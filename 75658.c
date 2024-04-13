string16 URLFixerUpper::SegmentURL(const string16& text,
                                   url_parse::Parsed* parts) {
  std::string text_utf8 = UTF16ToUTF8(text);
  url_parse::Parsed parts_utf8;
  std::string scheme_utf8 = SegmentURL(text_utf8, &parts_utf8);
  UTF8PartsToUTF16Parts(text_utf8, parts_utf8, parts);
  return UTF8ToUTF16(scheme_utf8);
}

GURL OmniboxEditModel::PermanentURL() {
  return URLFixerUpper::FixupURL(base::UTF16ToUTF8(permanent_text_),
                                 std::string());
}

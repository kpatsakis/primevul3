bool OmniboxEditModel::MaybeAcceptKeywordBySpace(
    const base::string16& new_text) {
  size_t keyword_length = new_text.length() - 1;
  return (paste_state_ == NONE) && is_keyword_hint_ && !keyword_.empty() &&
      inline_autocomplete_text_.empty() &&
      (keyword_.length() == keyword_length) &&
      IsSpaceCharForAcceptingKeyword(new_text[keyword_length]) &&
      !new_text.compare(0, keyword_length, keyword_, 0, keyword_length) &&
      AcceptKeyword(ENTERED_KEYWORD_MODE_VIA_SPACE_AT_END);
}

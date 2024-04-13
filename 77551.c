ACMatchClassifications HistoryQuickProvider::SpansFromTermMatch(
    const history::TermMatches& matches,
    size_t text_length,
    bool is_url) {
  ACMatchClassification::Style url_style =
      is_url ? ACMatchClassification::URL : ACMatchClassification::NONE;
  ACMatchClassifications spans;
  if (matches.empty()) {
    if (text_length)
      spans.push_back(ACMatchClassification(0, url_style));
    return spans;
  }
  if (matches[0].offset)
    spans.push_back(ACMatchClassification(0, url_style));
  size_t match_count = matches.size();
  for (size_t i = 0; i < match_count;) {
    size_t offset = matches[i].offset;
    spans.push_back(ACMatchClassification(offset,
        ACMatchClassification::MATCH | url_style));
    do {
      offset += matches[i].length;
      ++i;
    } while ((i < match_count) && (offset == matches[i].offset));
    if (offset < text_length)
      spans.push_back(ACMatchClassification(offset, url_style));
  }

  return spans;
}

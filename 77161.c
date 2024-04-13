void NetworkActionPredictor::RegisterTransitionalMatches(
    const string16& user_text,
    const AutocompleteResult& result) {
  if (user_text.length() < kMinimumUserTextLength)
    return;
  const string16 lower_user_text(base::i18n::ToLower(user_text));

  std::vector<TransitionalMatch>::iterator match_it =
      std::find(transitional_matches_.begin(), transitional_matches_.end(),
                lower_user_text);

  if (match_it == transitional_matches_.end()) {
    TransitionalMatch transitional_match;
    transitional_match.user_text = lower_user_text;
    match_it = transitional_matches_.insert(transitional_matches_.end(),
                                            transitional_match);
  }

  for (AutocompleteResult::const_iterator it = result.begin();
       it != result.end(); ++it) {
    if (std::find(match_it->urls.begin(), match_it->urls.end(),
                  it->destination_url) == match_it->urls.end()) {
      match_it->urls.push_back(it->destination_url);
    }
  }
}

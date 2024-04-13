void HistoryQuickProvider::DoAutocomplete() {
  string16 term_string = autocomplete_input_.text();
  ScoredHistoryMatches matches = GetIndex()->HistoryItemsForTerms(term_string);
  if (matches.empty())
    return;

  int max_match_score = (PreventInlineAutocomplete(autocomplete_input_) ||
      !matches.begin()->can_inline) ?
      (AutocompleteResult::kLowestDefaultScore - 1) :
      matches.begin()->raw_score;
  for (ScoredHistoryMatches::const_iterator match_iter = matches.begin();
       match_iter != matches.end(); ++match_iter) {
    const ScoredHistoryMatch& history_match(*match_iter);
    max_match_score = std::min(max_match_score, history_match.raw_score);
    matches_.push_back(QuickMatchToACMatch(history_match, max_match_score));
    max_match_score--;
  }
}

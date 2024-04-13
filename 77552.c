void HistoryQuickProvider::Start(const AutocompleteInput& input,
                                 bool minimal_changes) {
  matches_.clear();
  if (disabled_)
    return;

  if ((input.type() == AutocompleteInput::INVALID) ||
      (input.type() == AutocompleteInput::FORCED_QUERY) ||
      (input.matches_requested() == AutocompleteInput::BEST_MATCH &&
       input.prevent_inline_autocomplete()))
    return;

  autocomplete_input_ = input;

  if (GetIndex()) {
    base::TimeTicks start_time = base::TimeTicks::Now();
    DoAutocomplete();
    if (input.text().length() < 6) {
      base::TimeTicks end_time = base::TimeTicks::Now();
      std::string name = "HistoryQuickProvider.QueryIndexTime." +
          base::IntToString(input.text().length());
      base::Histogram* counter = base::Histogram::FactoryGet(
          name, 1, 1000, 50, base::Histogram::kUmaTargetedHistogramFlag);
      counter->Add(static_cast<int>((end_time - start_time).InMilliseconds()));
    }
    UpdateStarredStateOfMatches();
  }
}

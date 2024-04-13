NetworkActionPredictor::Action NetworkActionPredictor::RecommendAction(
    const string16& user_text,
    const AutocompleteMatch& match) const {
  bool is_in_db = false;
  const double confidence = CalculateConfidence(user_text, match, &is_in_db);
  DCHECK(confidence >= 0.0 && confidence <= 1.0);

  UMA_HISTOGRAM_BOOLEAN("NetworkActionPredictor.MatchIsInDb", is_in_db);

  if (is_in_db) {
    tracked_urls_.push_back(std::make_pair(match.destination_url, confidence));
    UMA_HISTOGRAM_COUNTS_100("NetworkActionPredictor.Confidence",
                             confidence * 100);
  }

  Action action = ACTION_NONE;
  for (int i = 0; i < LAST_PREDICT_ACTION; ++i) {
    if (confidence >= kConfidenceCutoff[i]) {
      action = static_cast<Action>(i);
      break;
    }
  }

  if (action == ACTION_PRERENDER && !prerender::IsOmniboxEnabled(profile_))
    action = ACTION_PRECONNECT;

  return action;
}

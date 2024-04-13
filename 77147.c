double NetworkActionPredictor::CalculateConfidence(
    const string16& user_text,
    const AutocompleteMatch& match,
    bool* is_in_db) const {
  const DBCacheKey key = { user_text, match.destination_url };

  *is_in_db = false;
  if (user_text.length() < kMinimumUserTextLength)
    return 0.0;

  const DBCacheMap::const_iterator iter = db_cache_.find(key);
  if (iter == db_cache_.end())
    return 0.0;

  *is_in_db = true;
  return CalculateConfidenceForDbEntry(iter);
}

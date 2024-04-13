double NetworkActionPredictor::CalculateConfidenceForDbEntry(
    DBCacheMap::const_iterator iter) const {
  const DBCacheValue& value = iter->second;
  if (value.number_of_hits < kMinimumNumberOfHits)
    return 0.0;

  const double number_of_hits = value.number_of_hits * hit_weight_;
  return number_of_hits / (number_of_hits + value.number_of_misses);
}

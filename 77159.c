void NetworkActionPredictor::OnOmniboxOpenedUrl(const AutocompleteLog& log) {
  if (log.text.length() < kMinimumUserTextLength)
    return;

  const AutocompleteMatch& match = log.result.match_at(log.selected_index);

  UMA_HISTOGRAM_BOOLEAN(
      StringPrintf("Prerender.OmniboxNavigationsCouldPrerender_%.1f",
                   get_hit_weight()).c_str(),
      prerender::IsOmniboxEnabled(profile_));

  const GURL& opened_url = match.destination_url;

  prerender::PrerenderManager* prerender_manager =
      prerender::PrerenderManagerFactory::GetForProfile(profile_);
  if (prerender_manager && !prerender_manager->IsPrerendering(opened_url))
    prerender_manager->CancelOmniboxPrerenders();

  const string16 lower_user_text(base::i18n::ToLower(log.text));

  BeginTransaction();
  for (std::vector<TransitionalMatch>::const_iterator it =
       transitional_matches_.begin(); it != transitional_matches_.end();
       ++it) {
    if (!StartsWith(lower_user_text, it->user_text, true))
      continue;

    for (std::vector<GURL>::const_iterator url_it = it->urls.begin();
         url_it != it->urls.end(); ++url_it) {
      DCHECK(it->user_text.length() >= kMinimumUserTextLength);
      const DBCacheKey key = { it->user_text, *url_it };
      const bool is_hit = (*url_it == opened_url);

      NetworkActionPredictorDatabase::Row row;
      row.user_text = key.user_text;
      row.url = key.url;

      DBCacheMap::iterator it = db_cache_.find(key);
      if (it == db_cache_.end()) {
        row.id = guid::GenerateGUID();
        row.number_of_hits = is_hit ? 1 : 0;
        row.number_of_misses = is_hit ? 0 : 1;

        AddRow(key, row);
      } else {
        DCHECK(db_id_cache_.find(key) != db_id_cache_.end());
        row.id = db_id_cache_.find(key)->second;
        row.number_of_hits = it->second.number_of_hits + (is_hit ? 1 : 0);
        row.number_of_misses = it->second.number_of_misses + (is_hit ? 0 : 1);

        UpdateRow(it, row);
      }
    }
  }
  CommitTransaction();

  ClearTransitionalMatches();

  for (std::vector<std::pair<GURL, double> >::const_iterator it =
       tracked_urls_.begin(); it != tracked_urls_.end();
       ++it) {
    if (opened_url == it->first) {
      UMA_HISTOGRAM_COUNTS_100("NetworkActionPredictor.AccurateCount",
                               it->second * 100);
    }
  }
  tracked_urls_.clear();
}

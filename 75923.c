bool TypedUrlModelAssociator::WriteToHistoryBackend(
    const TypedUrlTitleVector* titles,
    const TypedUrlVector* new_urls,
    const TypedUrlUpdateVector* updated_urls,
    const TypedUrlVisitVector* new_visits,
    const history::VisitVector* deleted_visits) {
  if (titles) {
    for (TypedUrlTitleVector::const_iterator title = titles->begin();
         title != titles->end(); ++title) {
      history_backend_->SetPageTitle(title->first, title->second);
    }
  }
  if (new_urls) {
#ifndef NDEBUG
    for (TypedUrlVector::const_iterator url = new_urls->begin();
         url != new_urls->end(); ++url) {
      DCHECK(IsAssociated(url->url().spec()));
    }
#endif
    history_backend_->AddPagesWithDetails(*new_urls, history::SOURCE_SYNCED);
  }
  if (updated_urls) {
    for (TypedUrlUpdateVector::const_iterator url = updated_urls->begin();
         url != updated_urls->end(); ++url) {
      DCHECK(url->second.visit_count());
      DCHECK(url->second.typed_count());
      DCHECK(IsAssociated(url->second.url().spec()));
      if (!history_backend_->UpdateURL(url->first, url->second)) {
        LOG(ERROR) << "Could not update page: " << url->second.url().spec();
        return false;
      }
    }
  }
  if (new_visits) {
    for (TypedUrlVisitVector::const_iterator visits = new_visits->begin();
         visits != new_visits->end(); ++visits) {
      DCHECK(IsAssociated(visits->first.spec()));
      if (!history_backend_->AddVisits(visits->first, visits->second,
                                       history::SOURCE_SYNCED)) {
        LOG(ERROR) << "Could not add visits.";
        return false;
      }
    }
  }
  if (deleted_visits) {
    if (!history_backend_->RemoveVisits(*deleted_visits)) {
      LOG(ERROR) << "Could not remove visits.";
      return false;
    }
  }
  return true;
}

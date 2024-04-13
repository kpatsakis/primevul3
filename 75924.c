void TypedUrlModelAssociator::WriteToSyncNode(
    const history::URLRow& url,
    const history::VisitVector& visits,
    sync_api::WriteNode* node) {
  DCHECK(!url.last_visit().is_null());
  DCHECK(!visits.empty());
  DCHECK(url.last_visit() == visits.back().visit_time);

  sync_pb::TypedUrlSpecifics typed_url;
  typed_url.set_url(url.url().spec());
  typed_url.set_title(UTF16ToUTF8(url.title()));
  typed_url.set_hidden(url.hidden());

  for (history::VisitVector::const_iterator visit = visits.begin();
       visit != visits.end(); ++visit) {
    typed_url.add_visits(visit->visit_time.ToInternalValue());
    typed_url.add_visit_transitions(
        visit->transition & PageTransition::CORE_MASK);
  }

  node->SetTypedUrlSpecifics(typed_url);
}

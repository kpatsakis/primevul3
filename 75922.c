void TypedUrlModelAssociator::UpdateURLRowFromTypedUrlSpecifics(
    const sync_pb::TypedUrlSpecifics& typed_url, history::URLRow* new_url) {
  DCHECK_GT(typed_url.visits_size(), 0);
  DCHECK_EQ(typed_url.visit_transitions_size(), typed_url.visits_size());
  new_url->set_title(UTF8ToUTF16(typed_url.title()));
  new_url->set_hidden(typed_url.hidden());
  new_url->set_last_visit(base::Time::FromInternalValue(
      typed_url.visits(typed_url.visits_size() - 1)));
}

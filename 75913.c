void TypedUrlModelAssociator::DiffVisits(
    const history::VisitVector& old_visits,
    const sync_pb::TypedUrlSpecifics& new_url,
    std::vector<history::VisitInfo>* new_visits,
    history::VisitVector* removed_visits) {
  size_t old_visit_count = old_visits.size();
  size_t new_visit_count = new_url.visits_size();
  size_t old_index = 0;
  size_t new_index = 0;
  while (old_index < old_visit_count && new_index < new_visit_count) {
    base::Time new_visit_time =
        base::Time::FromInternalValue(new_url.visits(new_index));
    if (old_visits[old_index].visit_time < new_visit_time) {
      removed_visits->push_back(old_visits[old_index]);
      ++old_index;
    } else if (old_visits[old_index].visit_time > new_visit_time) {
      new_visits->push_back(history::VisitInfo(
          new_visit_time, new_url.visit_transitions(new_index)));
      ++new_index;
    } else {
      ++old_index;
      ++new_index;
    }
  }

  for ( ; old_index < old_visit_count; ++old_index) {
    removed_visits->push_back(old_visits[old_index]);
  }

  for ( ; new_index < new_visit_count; ++new_index) {
    new_visits->push_back(history::VisitInfo(
        base::Time::FromInternalValue(new_url.visits(new_index)),
        new_url.visit_transitions(new_index)));
  }
}

void GetEventFiltersToNotify(Window* target, EventFilters* filters) {
  while (target) {
    if (target->event_filter())
      filters->push_back(target->event_filter());
    target = target->parent();
  }
}

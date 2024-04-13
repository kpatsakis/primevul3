bool RootWindow::ProcessKeyEvent(Window* target, KeyEvent* event) {
  EventFilters filters;

  if (!target) {
    target = this;
    GetEventFiltersToNotify(this, &filters);
  } else {
    if (!target->IsVisible())
      return false;
    GetEventFiltersToNotify(target->parent(), &filters);
  }

  for (EventFilters::const_reverse_iterator it = filters.rbegin(),
           rend = filters.rend();
       it != rend; ++it) {
    if ((*it)->PreHandleKeyEvent(target, event))
      return true;
  }

  if (!target->delegate())
    return false;
  return target->delegate()->OnKeyEvent(event);
}

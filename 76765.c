bool RootWindow::ProcessMouseEvent(Window* target, MouseEvent* event) {
  if (!target->IsVisible())
    return false;

  EventFilters filters;
  GetEventFiltersToNotify(target->parent(), &filters);
  for (EventFilters::const_reverse_iterator it = filters.rbegin(),
           rend = filters.rend();
       it != rend; ++it) {
    if ((*it)->PreHandleMouseEvent(target, event))
      return true;
  }

  if (!target->delegate())
    return false;
  return target->delegate()->OnMouseEvent(event);
}

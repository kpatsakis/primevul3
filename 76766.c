ui::TouchStatus RootWindow::ProcessTouchEvent(Window* target,
                                              TouchEvent* event) {
  if (!target->IsVisible())
    return ui::TOUCH_STATUS_UNKNOWN;

  EventFilters filters;
  if (target == this)
    GetEventFiltersToNotify(target, &filters);
  else
    GetEventFiltersToNotify(target->parent(), &filters);
  for (EventFilters::const_reverse_iterator it = filters.rbegin(),
           rend = filters.rend();
       it != rend; ++it) {
    ui::TouchStatus status = (*it)->PreHandleTouchEvent(target, event);
    if (status != ui::TOUCH_STATUS_UNKNOWN)
      return status;
  }

  if (target->delegate())
    return target->delegate()->OnTouchEvent(event);

  return ui::TOUCH_STATUS_UNKNOWN;
}

ui::GestureStatus RootWindow::ProcessGestureEvent(Window* target,
                                                  GestureEvent* event) {
  if (!target->IsVisible())
    return ui::GESTURE_STATUS_UNKNOWN;

  EventFilters filters;
  if (target == this)
    GetEventFiltersToNotify(target, &filters);
  else
    GetEventFiltersToNotify(target->parent(), &filters);
  ui::GestureStatus status = ui::GESTURE_STATUS_UNKNOWN;
  for (EventFilters::const_reverse_iterator it = filters.rbegin(),
           rend = filters.rend();
       it != rend; ++it) {
    status = (*it)->PreHandleGestureEvent(target, event);
    if (status != ui::GESTURE_STATUS_UNKNOWN)
      return status;
  }

  if (target->delegate())
    status = target->delegate()->OnGestureEvent(event);
  if (status == ui::GESTURE_STATUS_UNKNOWN) {
    const ui::EventType* types = NULL;
    bool generate_move = false;
    switch (event->type()) {
      case ui::ET_GESTURE_TAP:
      case ui::ET_GESTURE_DOUBLE_TAP:  // Double click is special cased below.
        generate_move = true;
        types = kTapTypes;
        break;

      case ui::ET_GESTURE_SCROLL_BEGIN:
        generate_move = true;
        types = kScrollBeginTypes;
        break;

      case ui::ET_GESTURE_SCROLL_UPDATE:
        types = kScrollUpdateTypes;
        break;

      case ui::ET_GESTURE_SCROLL_END:
        types = kScrollEndTypes;
        break;

      default:
        break;
    }
    if (types) {
      gfx::Point point_in_root(event->location());
      Window::ConvertPointToWindow(target, this, &point_in_root);
      if (generate_move && point_in_root != last_mouse_location_) {
        MouseEvent synth(ui::ET_MOUSE_MOVED, point_in_root,
                         event->root_location(), event->flags());
        if (DispatchMouseEventToTarget(&synth, target))
          status = ui::GESTURE_STATUS_SYNTH_MOUSE;
      }
      for (const ui::EventType* type = types; *type != ui::ET_UNKNOWN;
           ++type) {
        int flags = event->flags();
        if (event->type() == ui::ET_GESTURE_DOUBLE_TAP &&
            *type == ui::ET_MOUSE_PRESSED)
          flags |= ui::EF_IS_DOUBLE_CLICK;

        flags |= ui::EF_LEFT_MOUSE_BUTTON;

        MouseEvent synth(*type, point_in_root, event->root_location(), flags);
        if (DispatchMouseEventToTarget(&synth, target))
          status = ui::GESTURE_STATUS_SYNTH_MOUSE;
      }
    }
  }

  return status;
}

void RenderThreadImpl::WidgetRestored() {
  DCHECK_GT(hidden_widget_count_, 0);
  hidden_widget_count_--;

  if (!GetContentClient()->renderer()->RunIdleHandlerWhenWidgetsHidden()) {
    return;
  }

  ScheduleIdleHandler(kLongIdleHandlerDelayMs);
}

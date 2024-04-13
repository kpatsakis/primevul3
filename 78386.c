void RenderThreadImpl::WidgetHidden() {
  DCHECK_LT(hidden_widget_count_, widget_count_);
  hidden_widget_count_++;

  if (widget_count_ && hidden_widget_count_ == widget_count_) {
#if !defined(SYSTEM_NATIVELY_SIGNALS_MEMORY_PRESSURE)

#endif
    if (GetContentClient()->renderer()->RunIdleHandlerWhenWidgetsHidden())
      ScheduleIdleHandler(kInitialIdleHandlerDelayMs);
  }
}

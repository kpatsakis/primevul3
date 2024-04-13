void RenderThreadImpl::IdleHandler() {
  bool run_in_foreground_tab = (widget_count_ > hidden_widget_count_) &&
                               GetContentClient()->renderer()->
                                   RunIdleHandlerWhenWidgetsHidden();
  if (run_in_foreground_tab) {
    IdleHandlerInForegroundTab();
    return;
  }

  base::allocator::ReleaseFreeMemory();

  bool continue_timer = !webkit_shared_timer_suspended_;

  if (!v8::V8::IdleNotification()) {
    continue_timer = true;
  }
  if (!base::DiscardableMemory::ReduceMemoryUsage()) {
    continue_timer = true;
  }

  if (continue_timer) {
    ScheduleIdleHandler(idle_notification_delay_in_ms_ +
                        1000000 / (idle_notification_delay_in_ms_ + 2000));

  } else {
    idle_timer_.Stop();
  }

  FOR_EACH_OBSERVER(RenderProcessObserver, observers_, IdleNotification());
}

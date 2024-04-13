void RenderThreadImpl::IdleHandlerInForegroundTab() {
  int64 new_delay_ms = idle_notification_delay_in_ms_ +
                       1000000 / (idle_notification_delay_in_ms_ + 2000);
  if (new_delay_ms >= kLongIdleHandlerDelayMs)
    new_delay_ms = kShortIdleHandlerDelayMs;

  if (idle_notifications_to_skip_ > 0) {
    idle_notifications_to_skip_--;
  } else  {
    int cpu_usage = 0;
    Send(new ViewHostMsg_GetCPUUsage(&cpu_usage));
    int idle_hint = static_cast<int>(new_delay_ms / 10);
    if (cpu_usage < kIdleCPUUsageThresholdInPercents) {
      base::allocator::ReleaseFreeMemory();

      bool finished_idle_work = true;
      if (!v8::V8::IdleNotification(idle_hint))
        finished_idle_work = false;
      if (!base::DiscardableMemory::ReduceMemoryUsage())
        finished_idle_work = false;

      if (finished_idle_work)
        new_delay_ms = kLongIdleHandlerDelayMs;
    }
  }
  ScheduleIdleHandler(new_delay_ms);
}

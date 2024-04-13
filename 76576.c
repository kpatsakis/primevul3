void ChromotingInstance::UnregisterLoggingInstance() {
  base::AutoLock lock(g_logging_lock.Get());

  if (this != g_logging_instance.Get().get())
    return;

  g_has_logging_instance = false;
  g_logging_instance.Get().reset();
  g_logging_task_runner.Get() = NULL;

  VLOG(1) << "Unregistering global log handler";
}

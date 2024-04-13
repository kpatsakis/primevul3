void ChromotingInstance::RegisterLoggingInstance() {
  base::AutoLock lock(g_logging_lock.Get());

  g_logging_instance.Get() = weak_factory_.GetWeakPtr();
  g_logging_task_runner.Get() = plugin_task_runner_;
  g_has_logging_instance = true;
}

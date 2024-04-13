void ChromotingInstance::RegisterLogMessageHandler() {
  base::AutoLock lock(g_logging_lock.Get());

  VLOG(1) << "Registering global log handler";

  g_logging_old_handler = logging::GetLogMessageHandler();

  logging::SetLogMessageHandler(&LogToUI);
}

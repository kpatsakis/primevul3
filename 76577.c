ChromotingInstance::~ChromotingInstance() {
  DCHECK(plugin_task_runner_->BelongsToCurrentThread());

  UnregisterLoggingInstance();

  view_.reset();

  if (client_.get()) {
    client_->Stop(base::Bind(&PluginThreadTaskRunner::Quit,
                  plugin_task_runner_));
  } else {
    plugin_task_runner_->Quit();
  }

  plugin_task_runner_->DetachAndRunShutdownLoop();

  context_.Stop();
}

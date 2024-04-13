void ChromotingInstance::Disconnect() {
  DCHECK(plugin_task_runner_->BelongsToCurrentThread());

  view_.reset();

  LOG(INFO) << "Disconnecting from host.";
  if (client_.get()) {
    base::WaitableEvent done_event(true, false);
    client_->Stop(base::Bind(&base::WaitableEvent::Signal,
                             base::Unretained(&done_event)));
    done_event.Wait();
    client_.reset();
  }

  mouse_input_filter_.set_input_stub(NULL);
  host_connection_.reset();
}

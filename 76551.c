bool ChromotingInstance::HandleInputEvent(const pp::InputEvent& event) {
  DCHECK(plugin_task_runner_->BelongsToCurrentThread());

  if (!IsConnected())
    return false;

  mouse_input_filter_.set_output_size(view_->get_screen_size());

  return input_handler_.HandleInputEvent(event);
}

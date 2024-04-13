void AudioOutputController::Close(Task* closed_task) {
  DCHECK(closed_task);
  DCHECK(message_loop_);
  message_loop_->PostTask(
      FROM_HERE,
      NewRunnableMethod(this, &AudioOutputController::DoClose, closed_task));
}

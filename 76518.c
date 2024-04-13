scoped_refptr<AudioOutputController> AudioOutputController::CreateLowLatency(
    EventHandler* event_handler,
    AudioParameters params,
    SyncReader* sync_reader) {

  DCHECK(sync_reader);

  if (!CheckParameters(params))
    return NULL;

  if (!AudioManager::GetAudioManager())
    return NULL;

  scoped_refptr<AudioOutputController> controller(new AudioOutputController(
      event_handler, 0, sync_reader));

  controller->message_loop_ =
      AudioManager::GetAudioManager()->GetMessageLoop();
  controller->message_loop_->PostTask(
      FROM_HERE,
      NewRunnableMethod(controller.get(), &AudioOutputController::DoCreate,
                        params));
  return controller;
}

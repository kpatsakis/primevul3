OmniboxViewWin::ScopedSuspendUndo::~ScopedSuspendUndo() {
  if (text_object_model_)
    text_object_model_->Undo(tomResume, NULL);
}

OmniboxViewWin::ScopedSuspendUndo::ScopedSuspendUndo(
    ITextDocument* text_object_model)
    : text_object_model_(text_object_model) {
  if (text_object_model_)
    text_object_model_->Undo(tomSuspend, NULL);
}

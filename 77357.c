OmniboxViewWin::ScopedFreeze::~ScopedFreeze() {
  if (edit_->IsWindow() && text_object_model_) {
    long count;
    text_object_model_->Unfreeze(&count);
    if (count == 0) {
      edit_->InvalidateRect(NULL, false);
      edit_->UpdateWindow();
    }
  }
}

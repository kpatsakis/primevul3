void OmniboxEditModel::OnUpOrDownKeyPressed(int count) {
  if (popup_model() && popup_model()->IsOpen()) {
    popup_model()->Move(count);
    return;
  }

  if (!query_in_progress()) {
    if (!user_input_in_progress_)
      InternalSetUserText(permanent_text_);
    view_->UpdatePopup();
    return;
  }

}

void RenderView::didReceiveDocumentData(
    WebFrame* frame, const char* data, size_t data_len,
    bool& prevent_default) {
  NavigationState* navigation_state =
      NavigationState::FromDataSource(frame->dataSource());
  if (!navigation_state->postpone_loading_data())
    return;

  prevent_default = true;

  navigation_state->append_postponed_data(data, data_len);
  if (navigation_state->postponed_data().size() >= 512) {
    navigation_state->set_postpone_loading_data(false);
    frame->commitDocumentData(navigation_state->postponed_data().data(),
                              navigation_state->postponed_data().size());
    navigation_state->clear_postponed_data();
  }
}

DWORD OmniboxViewWin::EditDropTarget::OnDragEnter(IDataObject* data_object,
                                                  DWORD key_state,
                                                  POINT cursor_position,
                                                  DWORD effect) {
  ui::OSExchangeData os_data(new ui::OSExchangeDataProviderWin(data_object));
  drag_has_url_ = os_data.HasURL();
  drag_has_string_ = !drag_has_url_ && os_data.HasString();
  if (drag_has_url_) {
    if (edit_->in_drag()) {
      drag_has_url_ = false;
    }
  }
  return OnDragOver(data_object, key_state, cursor_position, effect);
}

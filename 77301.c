DWORD OmniboxViewWin::EditDropTarget::OnDrop(IDataObject* data_object,
                                             DWORD key_state,
                                             POINT cursor_position,
                                             DWORD effect) {
  effect = OnDragOver(data_object, key_state, cursor_position, effect);

  ui::OSExchangeData os_data(new ui::OSExchangeDataProviderWin(data_object));
  views::DropTargetEvent event(os_data, cursor_position.x, cursor_position.y,
      ui::DragDropTypes::DropEffectToDragOperation(effect));

  int drag_operation = edit_->OnPerformDropImpl(event, edit_->in_drag());

  if (!drag_has_url_)
    ResetDropHighlights();

  return ui::DragDropTypes::DragOperationToDropEffect(drag_operation);
}

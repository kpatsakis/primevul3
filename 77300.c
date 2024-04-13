DWORD OmniboxViewWin::EditDropTarget::OnDragOver(IDataObject* data_object,
                                                 DWORD key_state,
                                                 POINT cursor_position,
                                                 DWORD effect) {
  if (drag_has_url_)
    return CopyOrLinkDropEffect(effect);

  if (drag_has_string_) {
    UpdateDropHighlightPosition(cursor_position);
    if (edit_->drop_highlight_position() == -1 && edit_->in_drag())
      return DROPEFFECT_NONE;
    if (edit_->in_drag()) {
      DCHECK((effect & DROPEFFECT_COPY) && (effect & DROPEFFECT_MOVE));
      return (key_state & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
    }
    return CopyOrLinkDropEffect(effect);
  }

  return DROPEFFECT_NONE;
}

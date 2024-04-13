void OmniboxEditModel::SetCaretVisibility(bool visible) {
  if (focus_state_ != OMNIBOX_FOCUS_NONE) {
    SetFocusState(visible ? OMNIBOX_FOCUS_VISIBLE : OMNIBOX_FOCUS_INVISIBLE,
                  OMNIBOX_FOCUS_CHANGE_EXPLICIT);
  }
}

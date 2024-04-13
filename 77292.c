bool OmniboxViewWin::OnAfterPossibleChangeInternal(bool force_text_changed) {
  CHARRANGE new_sel;
  GetSelection(new_sel);
  const int length = GetTextLength();
  if ((new_sel.cpMin > length) || (new_sel.cpMax > length)) {
    if (new_sel.cpMin > length)
      new_sel.cpMin = length;
    if (new_sel.cpMax > length)
      new_sel.cpMax = length;
    SetSelectionRange(new_sel);
  }
  const bool selection_differs =
      ((new_sel.cpMin != new_sel.cpMax) ||
       (sel_before_change_.cpMin != sel_before_change_.cpMax)) &&
      ((new_sel.cpMin != sel_before_change_.cpMin) ||
       (new_sel.cpMax != sel_before_change_.cpMax));

  const string16 new_text(GetText());
  const bool text_differs = (new_text != text_before_change_) ||
      force_text_changed;

  const bool just_deleted_text =
      (text_before_change_.length() > new_text.length()) &&
      (new_sel.cpMin <= std::min(sel_before_change_.cpMin,
                                 sel_before_change_.cpMax));

  const bool something_changed = model_->OnAfterPossibleChange(
      new_text, new_sel.cpMin, new_sel.cpMax, selection_differs,
      text_differs, just_deleted_text, !IsImeComposing());

  if (selection_differs)
    controller_->OnSelectionBoundsChanged();

  if (something_changed && text_differs)
    TextChanged();

  if (text_differs) {
    native_view_host_->GetWidget()->NotifyAccessibilityEvent(
        native_view_host_, ui::AccessibilityTypes::EVENT_TEXT_CHANGED, true);
  } else if (selection_differs) {
    native_view_host_->GetWidget()->NotifyAccessibilityEvent(
        native_view_host_,
        ui::AccessibilityTypes::EVENT_SELECTION_CHANGED,
        true);
  } else if (delete_at_end_pressed_) {
    model_->OnChanged();
  }

  return something_changed;
}

bool OmniboxEditModel::OnAfterPossibleChange(const base::string16& old_text,
                                             const base::string16& new_text,
                                             size_t selection_start,
                                             size_t selection_end,
                                             bool selection_differs,
                                             bool text_differs,
                                             bool just_deleted_text,
                                             bool allow_keyword_ui_change) {
  if (paste_state_ == PASTING)
    paste_state_ = PASTED;
  else if (text_differs)
    paste_state_ = NONE;

  if (text_differs || selection_differs) {
    if (focus_source_ == INVALID) {
      focus_source_ = (focus_state_ == OMNIBOX_FOCUS_INVISIBLE) ?
          FAKEBOX : OMNIBOX;
    }

    SetFocusState(OMNIBOX_FOCUS_VISIBLE, OMNIBOX_FOCUS_CHANGE_TYPING);
  }

  const bool user_text_changed =
      text_differs || (selection_differs && !inline_autocomplete_text_.empty());

  if ((text_differs || selection_differs) &&
      (control_key_state_ == DOWN_WITHOUT_CHANGE))
    control_key_state_ = DOWN_WITH_CHANGE;

  if (!user_text_changed)
    return false;

  InternalSetUserText(UserTextFromDisplayText(new_text));
  has_temporary_text_ = false;

  just_deleted_text_ = just_deleted_text;

  if (user_input_in_progress_ && user_text_.empty()) {
    UMA_HISTOGRAM_ENUMERATION(kOmniboxUserTextClearedHistogram,
                              OMNIBOX_USER_TEXT_CLEARED_BY_EDITING,
                              OMNIBOX_USER_TEXT_CLEARED_NUM_OF_ITEMS);
  }

  const bool no_selection = selection_start == selection_end;

  allow_exact_keyword_match_ = text_differs && allow_keyword_ui_change &&
      !just_deleted_text && no_selection &&
      CreatedKeywordSearchByInsertingSpaceInMiddle(old_text, user_text_,
                                                   selection_start);
  if (allow_exact_keyword_match_) {
    UMA_HISTOGRAM_ENUMERATION(kEnteredKeywordModeHistogram,
                              ENTERED_KEYWORD_MODE_VIA_SPACE_IN_MIDDLE,
                              ENTERED_KEYWORD_MODE_NUM_ITEMS);
  }
  view_->UpdatePopup();
  allow_exact_keyword_match_ = false;

  return !(text_differs && allow_keyword_ui_change && !just_deleted_text &&
           no_selection && (selection_start == user_text_.length()) &&
           MaybeAcceptKeywordBySpace(user_text_));
}

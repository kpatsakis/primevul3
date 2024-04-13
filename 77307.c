bool OmniboxViewWin::OnKeyDownOnlyWritable(TCHAR key,
                                           UINT repeat_count,
                                           UINT flags) {

  int count = repeat_count;
  switch (key) {
    case VK_RIGHT:
      if (base::i18n::IsRTL())
        return false;
      {
        CHARRANGE selection;
        GetSel(selection);
        return (selection.cpMin == selection.cpMax) &&
            (selection.cpMin == GetTextLength()) &&
            model_->CommitSuggestedText(true);
      }

    case VK_RETURN:
      model_->AcceptInput((flags & KF_ALTDOWN) ?
          NEW_FOREGROUND_TAB : CURRENT_TAB, false);
      return true;

    case VK_PRIOR:
    case VK_NEXT:
      count = model_->result().size();
    case VK_UP:
    case VK_DOWN:
      if ((flags & KF_ALTDOWN) && !(flags & KF_EXTENDED))
        return false;

      model_->OnUpOrDownKeyPressed(((key == VK_PRIOR) || (key == VK_UP)) ?
          -count : count);
      return true;


    case VK_DELETE:
      if (flags & KF_ALTDOWN)
        return false;
      if (GetKeyState(VK_SHIFT) >= 0) {
        if (GetKeyState(VK_CONTROL) >= 0) {
          CHARRANGE selection;
          GetSel(selection);
          delete_at_end_pressed_ = ((selection.cpMin == selection.cpMax) &&
                                    (selection.cpMin == GetTextLength()));
        }
        return false;
      }
      if (GetKeyState(VK_CONTROL) >= 0) {
        CHARRANGE selection;
        GetSel(selection);
        if (selection.cpMin != selection.cpMax) {
          ScopedFreeze freeze(this, GetTextObjectModel());
          OnBeforePossibleChange();
          Cut();
          OnAfterPossibleChange();
        } else {
          if (model_->popup_model()->IsOpen()) {
            model_->popup_model()->TryDeletingCurrentItem();
          }
        }
      }
      return true;

    case 'X':
      if ((flags & KF_ALTDOWN) || (GetKeyState(VK_CONTROL) >= 0))
        return false;
      if (GetKeyState(VK_SHIFT) >= 0) {
        ScopedFreeze freeze(this, GetTextObjectModel());
        OnBeforePossibleChange();
        Cut();
        OnAfterPossibleChange();
      }
      return true;

    case VK_INSERT:
      if (!(flags & KF_ALTDOWN) && (GetKeyState(VK_SHIFT) >= 0) &&
          (GetKeyState(VK_CONTROL) >= 0))
        return true;
    case 'V':
      if ((flags & KF_ALTDOWN) ||
          (GetKeyState((key == 'V') ? VK_CONTROL : VK_SHIFT) >= 0))
        return false;
      if (GetKeyState((key == 'V') ? VK_SHIFT : VK_CONTROL) >= 0) {
        ScopedFreeze freeze(this, GetTextObjectModel());
        OnBeforePossibleChange();
        Paste();
        OnAfterPossibleChange();
      }
      return true;

    case VK_BACK: {
      if ((flags & KF_ALTDOWN) || model_->is_keyword_hint() ||
          model_->keyword().empty())
        return false;

      {
        CHARRANGE selection;
        GetSel(selection);
        if ((selection.cpMin != selection.cpMax) || (selection.cpMin != 0))
          return false;
      }

      ScopedFreeze freeze(this, GetTextObjectModel());
      model_->ClearKeyword(GetText());
      return true;
    }

    case VK_TAB: {
      if (model_->is_keyword_hint()) {
        ScopedFreeze freeze(this, GetTextObjectModel());
        model_->AcceptKeyword();
      } else if (!IsCaretAtEnd()) {
        ScopedFreeze freeze(this, GetTextObjectModel());
        OnBeforePossibleChange();
        PlaceCaretAt(GetTextLength());
        OnAfterPossibleChange();
      } else {
        model_->CommitSuggestedText(true);
      }
      return true;
    }

    case 0xbb:  // Ctrl-'='.  Triggers subscripting (even in plain text mode).
      return true;

    default:
      return false;
  }
}

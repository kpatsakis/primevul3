void OmniboxViewWin::ExecuteCommand(int command_id) {
  ScopedFreeze freeze(this, GetTextObjectModel());
  if (command_id == IDS_PASTE_AND_GO) {
    model_->PasteAndGo();
    return;
  }

  OnBeforePossibleChange();
  switch (command_id) {
    case IDS_UNDO:
      Undo();
      break;

    case IDC_CUT:
      Cut();
      break;

    case IDC_COPY:
      Copy();
      break;

    case IDC_PASTE:
      Paste();
      break;

    case IDS_SELECT_ALL:
      SelectAll(false);
      break;

    case IDS_EDIT_SEARCH_ENGINES:
      command_updater_->ExecuteCommand(IDC_EDIT_SEARCH_ENGINES);
      break;

    default:
      NOTREACHED();
      break;
  }
  OnAfterPossibleChange();
}

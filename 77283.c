bool OmniboxViewWin::IsCommandIdEnabled(int command_id) const {
  switch (command_id) {
    case IDS_UNDO:         return !!CanUndo();
    case IDC_CUT:          return !!CanCut();
    case IDC_COPY:         return !!CanCopy();
    case IDC_PASTE:        return !!CanPaste();
    case IDS_PASTE_AND_GO: return CanPasteAndGo(GetClipboardText());
    case IDS_SELECT_ALL:   return !!CanSelectAll();
    case IDS_EDIT_SEARCH_ENGINES:
      return command_updater_->IsCommandEnabled(IDC_EDIT_SEARCH_ENGINES);
    default:
      NOTREACHED();
      return false;
  }
}

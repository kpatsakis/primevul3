void OmniboxViewWin::BuildContextMenu() {
  if (context_menu_contents_.get())
    return;

  context_menu_contents_.reset(new ui::SimpleMenuModel(this));
  if (popup_window_mode_) {
    context_menu_contents_->AddItemWithStringId(IDC_COPY, IDS_COPY);
  } else {
    context_menu_contents_->AddItemWithStringId(IDS_UNDO, IDS_UNDO);
    context_menu_contents_->AddSeparator();
    context_menu_contents_->AddItemWithStringId(IDC_CUT, IDS_CUT);
    context_menu_contents_->AddItemWithStringId(IDC_COPY, IDS_COPY);
    context_menu_contents_->AddItemWithStringId(IDC_PASTE, IDS_PASTE);
    context_menu_contents_->AddItemWithStringId(IDS_PASTE_AND_GO,
                                                IDS_PASTE_AND_GO);
    context_menu_contents_->AddSeparator();
    context_menu_contents_->AddItemWithStringId(IDS_SELECT_ALL, IDS_SELECT_ALL);
    context_menu_contents_->AddSeparator();
    context_menu_contents_->AddItemWithStringId(IDS_EDIT_SEARCH_ENGINES,
                                                IDS_EDIT_SEARCH_ENGINES);
  }
  context_menu_.reset(new views::Menu2(context_menu_contents_.get()));
}

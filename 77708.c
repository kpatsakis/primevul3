void BookmarkManagerView::SetTableModel(BookmarkTableModel* new_table_model,
                                        BookmarkNode* parent_node,
                                        bool is_search) {
  table_view_->SetModel(NULL);
  table_view_->SetShowPathColumn(!parent_node);
  table_view_->SetModel(new_table_model);
  table_view_->set_parent_node(parent_node);
  table_model_.reset(new_table_model);
  if (!is_search || (new_table_model && new_table_model->RowCount() > 0)) {
    table_view_->SetAltText(std::wstring());
  } else if (search_tf_->GetText().empty()) {
    table_view_->SetAltText(
        l10n_util::GetString(IDS_BOOKMARK_MANAGER_NO_SEARCH_TEXT));
  } else {
    table_view_->SetAltText(
        l10n_util::GetStringF(IDS_BOOKMARK_MANAGER_NO_RESULTS,
                              search_tf_->GetText()));
  }
}

BookmarkTableModel* BookmarkManagerView::CreateSearchTableModel() {
  std::wstring search_text = search_tf_->GetText();
  if (search_text.empty())
    return NULL;
  return BookmarkTableModel::CreateSearchTableModel(GetBookmarkModel(),
                                                    search_text);
}

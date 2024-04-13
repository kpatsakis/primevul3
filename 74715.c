void AutoFillHelper::SuggestionsReceived(int query_id,
                                         const std::vector<string16>& values,
                                         const std::vector<string16>& labels,
                                         const std::vector<string16>& icons,
                                         const std::vector<int>& unique_ids) {
  WebKit::WebView* web_view = render_view_->webview();
  if (!web_view || query_id != autofill_query_id_)
    return;

  web_view->hidePopups();

  if (values.empty())
    return;

  std::vector<string16> v(values);
  std::vector<string16> l(labels);
  std::vector<string16> i(icons);
  std::vector<int> ids(unique_ids);
  int separator_index = -1;

  if (form_manager_.FormWithNodeIsAutoFilled(autofill_query_node_)) {
    v.push_back(l10n_util::GetStringUTF16(IDS_AUTOFILL_CLEAR_FORM_MENU_ITEM));
    l.push_back(string16());
    i.push_back(string16());
    ids.push_back(0);
    suggestions_clear_index_ = v.size() - 1;
    separator_index = values.size();
  }

  bool show_options = false;
  for (size_t i = 0; i < ids.size(); ++i) {
    if (ids[i] != 0) {
      show_options = true;
      break;
    }
  }
  if (show_options) {
    v.push_back(l10n_util::GetStringUTF16(IDS_AUTOFILL_OPTIONS));
    l.push_back(string16());
    i.push_back(string16());
    ids.push_back(0);
    suggestions_options_index_ = v.size() - 1;
    separator_index = values.size();
  }

  if (!v.empty()) {
    web_view->applyAutoFillSuggestions(
        autofill_query_node_, v, l, i, ids, separator_index);
  }
}

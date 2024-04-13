void RenderView::OnAutoFillSuggestionsReturned(
    int query_id,
    const std::vector<string16>& values,
    const std::vector<string16>& labels,
    const std::vector<string16>& icons,
    const std::vector<int>& unique_ids) {
  autofill_helper_.SuggestionsReceived(
      query_id, values, labels, icons, unique_ids);
}

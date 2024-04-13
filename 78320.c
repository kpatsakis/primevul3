bool OmniboxEditModel::UpdatePermanentText() {
  SearchProvider* search_provider =
      autocomplete_controller()->search_provider();
  if (search_provider && delegate_->CurrentPageExists())
    search_provider->set_current_page_url(delegate_->GetURL());

  base::string16 new_permanent_text = controller_->GetToolbarModel()->GetText();
  base::string16 gray_text = view_->GetGrayTextAutocompletion();
  const bool visibly_changed_permanent_text =
      (permanent_text_ != new_permanent_text) &&
      (!has_focus() ||
       (!user_input_in_progress_ &&
        !(popup_model() && popup_model()->IsOpen()) &&
        controller_->GetToolbarModel()->url_replacement_enabled())) &&
      (gray_text.empty() ||
       new_permanent_text != user_text_ + gray_text);

  permanent_text_ = new_permanent_text;
  return visibly_changed_permanent_text;
}

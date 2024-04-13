OmniboxEditModel::State::State(bool user_input_in_progress,
                               const base::string16& user_text,
                               const base::string16& gray_text,
                               const base::string16& keyword,
                               bool is_keyword_hint,
                               bool url_replacement_enabled,
                               OmniboxFocusState focus_state,
                               FocusSource focus_source,
                               const AutocompleteInput& autocomplete_input)
    : user_input_in_progress(user_input_in_progress),
      user_text(user_text),
      gray_text(gray_text),
      keyword(keyword),
      is_keyword_hint(is_keyword_hint),
      url_replacement_enabled(url_replacement_enabled),
      focus_state(focus_state),
      focus_source(focus_source),
      autocomplete_input(autocomplete_input) {
}

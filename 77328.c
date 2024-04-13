void OmniboxViewWin::OpenMatch(const AutocompleteMatch& match,
                               WindowOpenDisposition disposition,
                               const GURL& alternate_nav_url,
                               size_t selected_line,
                               const string16& keyword) {
  if (!match.destination_url.is_valid())
    return;

  ScopedFreeze freeze(this, GetTextObjectModel());
  model_->OpenMatch(match, disposition, alternate_nav_url,
                    selected_line, keyword);
}

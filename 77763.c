CurrentHistoryCleaner::CurrentHistoryCleaner(content::WebContents* contents)
    : WebContentsObserver(contents) {
  history_index_to_remove_ =
      web_contents()->GetController().GetLastCommittedEntryIndex();
}

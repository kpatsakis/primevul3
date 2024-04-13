GtkClipboard* Clipboard::LookupBackingClipboard(Buffer clipboard) const {
  switch (clipboard) {
    case BUFFER_STANDARD:
      return clipboard_;
    case BUFFER_SELECTION:
      return primary_selection_;
    default:
      NOTREACHED();
      return NULL;
  }
}

void Clipboard::ReadAsciiText(Clipboard::Buffer buffer,
                              std::string* result) const {
  GtkClipboard* clipboard = LookupBackingClipboard(buffer);
  if (clipboard == NULL)
    return;

  result->clear();
  gchar* text = gtk_clipboard_wait_for_text(clipboard);

  if (text == NULL)
    return;

  result->assign(text);
  g_free(text);
}

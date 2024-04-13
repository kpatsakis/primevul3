void Clipboard::ReadText(Clipboard::Buffer buffer, string16* result) const {
  GtkClipboard* clipboard = LookupBackingClipboard(buffer);
  if (clipboard == NULL)
    return;

  result->clear();
  gchar* text = gtk_clipboard_wait_for_text(clipboard);

  if (text == NULL)
    return;

  UTF8ToUTF16(text, strlen(text), result);
  g_free(text);
}

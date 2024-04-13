bool Clipboard::IsFormatAvailable(const Clipboard::FormatType& format,
                                  Clipboard::Buffer buffer) const {
  GtkClipboard* clipboard = LookupBackingClipboard(buffer);
  if (clipboard == NULL)
    return false;

  bool format_is_plain_text = GetPlainTextFormatType() == format;
  if (format_is_plain_text) {
    if (gtk_clipboard_wait_is_text_available(clipboard))
      return true;
  }

  bool retval = false;
  GdkAtom* targets = NULL;
  GtkSelectionData* data =
      gtk_clipboard_wait_for_contents(clipboard,
                                      gdk_atom_intern("TARGETS", false));

  if (!data)
    return false;

  int num = 0;
  gtk_selection_data_get_targets(data, &targets, &num);

  if (num <= 0) {
    if (format_is_plain_text) {
      gchar* text = gtk_clipboard_wait_for_text(clipboard);
      if (text) {
        g_free(text);
        retval = true;
      }
    }
  }

  GdkAtom format_atom = StringToGdkAtom(format);

  for (int i = 0; i < num; i++) {
    if (targets[i] == format_atom) {
      retval = true;
      break;
    }
  }

  g_free(targets);
  gtk_selection_data_free(data);

  return retval;
}

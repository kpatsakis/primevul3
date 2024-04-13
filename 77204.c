void Clipboard::ReadHTML(Clipboard::Buffer buffer, string16* markup,
                         std::string* src_url, uint32* fragment_start,
                         uint32* fragment_end) const {
  markup->clear();
  if (src_url)
    src_url->clear();
  *fragment_start = 0;
  *fragment_end = 0;

  GtkClipboard* clipboard = LookupBackingClipboard(buffer);
  if (clipboard == NULL)
    return;
  GtkSelectionData* data = gtk_clipboard_wait_for_contents(clipboard,
      StringToGdkAtom(GetHtmlFormatType()));

  if (!data)
    return;

  if (data->length >= 2 &&
      reinterpret_cast<uint16_t*>(data->data)[0] == 0xFEFF) {
    markup->assign(reinterpret_cast<uint16_t*>(data->data) + 1,
                   (data->length / 2) - 1);
  } else {
    UTF8ToUTF16(reinterpret_cast<char*>(data->data), data->length, markup);
  }

  if (!markup->empty() && markup->at(markup->length() - 1) == '\0')
    markup->resize(markup->length() - 1);

  *fragment_start = 0;
  DCHECK(markup->length() <= kuint32max);
  *fragment_end = static_cast<uint32>(markup->length());

  gtk_selection_data_free(data);
}

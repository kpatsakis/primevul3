void Clipboard::ReadData(const std::string& format, std::string* result) {
  GtkSelectionData* data =
      gtk_clipboard_wait_for_contents(clipboard_, StringToGdkAtom(format));
  if (!data)
    return;
  result->assign(reinterpret_cast<char*>(data->data), data->length);
  gtk_selection_data_free(data);
 }

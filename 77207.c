void Clipboard::SetGtkClipboard() {
  scoped_array<GtkTargetEntry> targets(
      new GtkTargetEntry[clipboard_data_->size()]);

  int i = 0;
  for (Clipboard::TargetMap::iterator iter = clipboard_data_->begin();
       iter != clipboard_data_->end(); ++iter, ++i) {
    targets[i].target = const_cast<char*>(iter->first.c_str());
    targets[i].flags = 0;
    targets[i].info = 0;
  }

  if (gtk_clipboard_set_with_data(clipboard_, targets.get(),
                                  clipboard_data_->size(),
                                  GetData, ClearData,
                                  clipboard_data_)) {
    gtk_clipboard_set_can_store(clipboard_,
                                targets.get(),
                                clipboard_data_->size());
  }

  clipboard_data_ = NULL;
}

Clipboard::~Clipboard() {
  gtk_clipboard_store(clipboard_);
}

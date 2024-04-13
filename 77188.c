Clipboard::Clipboard() : clipboard_data_(NULL) {
#if !defined(USE_AURA)
  clipboard_ = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
  primary_selection_ = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
#endif
}

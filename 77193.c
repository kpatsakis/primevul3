Clipboard::FormatType Clipboard::GetPlainTextFormatType() {
  return GdkAtomToString(GDK_TARGET_STRING);
}

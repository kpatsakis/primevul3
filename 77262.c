string16 OmniboxViewWin::GetClipboardText() const {
  ui::Clipboard* clipboard = g_browser_process->clipboard();
  if (clipboard->IsFormatAvailable(ui::Clipboard::GetPlainTextWFormatType(),
                                   ui::Clipboard::BUFFER_STANDARD)) {
    string16 text;
    clipboard->ReadText(ui::Clipboard::BUFFER_STANDARD, &text);
    return StripJavascriptSchemas(CollapseWhitespace(text, true));
  }

  if (clipboard->IsFormatAvailable(ui::Clipboard::GetUrlWFormatType(),
                                   ui::Clipboard::BUFFER_STANDARD)) {
    std::string url_str;
    clipboard->ReadBookmark(NULL, &url_str);
    GURL url(url_str);
    if (url.is_valid())
      return StripJavascriptSchemas(UTF8ToUTF16(url.spec()));
  }

  return string16();
}

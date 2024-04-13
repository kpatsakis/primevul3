void RenderThreadImpl::PreCacheFontCharacters(const LOGFONT& log_font,
                                              const base::string16& str) {
  Send(new ViewHostMsg_PreCacheFontCharacters(log_font, str));
}

void OmniboxViewWin::OnCopy() {
  string16 text(GetSelectedText());
  if (text.empty())
    return;

  CHARRANGE sel;
  GURL url;
  bool write_url = false;
  GetSel(sel);
  model_->AdjustTextForCopy(sel.cpMin, IsSelectAll(), &text, &url, &write_url);
  ui::ScopedClipboardWriter scw(g_browser_process->clipboard());
  scw.WriteText(text);
  if (write_url) {
    scw.WriteBookmark(text, url.spec());
    scw.WriteHyperlink(net::EscapeForHTML(text), url.spec());
  }
}

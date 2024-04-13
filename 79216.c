void DevToolsWindow::UpdateTheme() {
  ThemeService* tp = ThemeServiceFactory::GetForProfile(profile_);
  DCHECK(tp);

  std::string command("InspectorFrontendAPI.setToolbarColors(\"" +
      SkColorToRGBAString(tp->GetColor(ThemeProperties::COLOR_TOOLBAR)) +
      "\", \"" +
      SkColorToRGBAString(tp->GetColor(ThemeProperties::COLOR_BOOKMARK_TEXT)) +
      "\")");
  web_contents_->GetRenderViewHost()->ExecuteJavascriptInWebFrame(
      base::string16(), base::ASCIIToUTF16(command));
}

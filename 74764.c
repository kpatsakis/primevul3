void RenderView::InjectToolstripCSS() {
  if (view_type_ != ViewType::EXTENSION_TOOLSTRIP)
    return;

  static const base::StringPiece toolstrip_css(
      ResourceBundle::GetSharedInstance().GetRawDataResource(
      IDR_EXTENSION_TOOLSTRIP_CSS));
  std::string css = toolstrip_css.as_string();
  InsertCSS(L"", css, "ToolstripDefaultCSS");
}

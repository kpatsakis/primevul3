std::string RenderView::GetAltHTMLForTemplate(
    const DictionaryValue& error_strings, int template_resource_id) const {
  const base::StringPiece template_html(
      ResourceBundle::GetSharedInstance().GetRawDataResource(
          template_resource_id));

  if (template_html.empty()) {
    NOTREACHED() << "unable to load template. ID: " << template_resource_id;
    return "";
  }

  return jstemplate_builder::GetTemplatesHtml(
      template_html, &error_strings, "t");
}

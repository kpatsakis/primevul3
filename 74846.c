void RenderView::OnTranslatePage(int page_id,
                                 const std::string& translate_script,
                                 const std::string& source_lang,
                                 const std::string& target_lang) {
  translate_helper_.TranslatePage(page_id, source_lang, target_lang,
                                  translate_script);
}

WebString RenderView::autoCorrectWord(const WebKit::WebString& word) {
  string16 autocorrect_word;
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kExperimentalSpellcheckerFeatures)) {
    EnsureDocumentTag();
    RenderThread* thread = RenderThread::current();
    if (thread) {
      autocorrect_word =
          thread->spellchecker()->GetAutoCorrectionWord(
              word, document_tag_);
    }
  }
  return autocorrect_word;
}

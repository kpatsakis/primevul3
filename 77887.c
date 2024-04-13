blink::WebColorChooser* RenderFrameImpl::createColorChooser(
    blink::WebColorChooserClient* client,
    const blink::WebColor& initial_color,
    const blink::WebVector<blink::WebColorSuggestion>& suggestions) {
  RendererWebColorChooserImpl* color_chooser =
      new RendererWebColorChooserImpl(this, client);
  std::vector<content::ColorSuggestion> color_suggestions;
  for (size_t i = 0; i < suggestions.size(); i++) {
    color_suggestions.push_back(content::ColorSuggestion(suggestions[i]));
  }
  color_chooser->Open(static_cast<SkColor>(initial_color), color_suggestions);
  return color_chooser;
}

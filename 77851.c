void RenderFrameImpl::OnImeSetComposition(
    const base::string16& text,
    const std::vector<blink::WebCompositionUnderline>& underlines,
    int selection_start,
    int selection_end) {
  if (!IsPepperAcceptingCompositionEvents()) {
    pepper_composition_text_ = text;
  } else {

    if (pepper_composition_text_.empty() && !text.empty()) {
      render_view_->focused_pepper_plugin()->HandleCompositionStart(
          base::string16());
    }
    if (!pepper_composition_text_.empty() && text.empty()) {
      render_view_->focused_pepper_plugin()->HandleCompositionEnd(
          base::string16());
    }
    pepper_composition_text_ = text;
    if (!pepper_composition_text_.empty()) {
      render_view_->focused_pepper_plugin()->HandleCompositionUpdate(
          pepper_composition_text_, underlines, selection_start,
          selection_end);
    }
  }
}

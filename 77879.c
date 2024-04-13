void RenderFrameImpl::SimulateImeConfirmComposition(
    const base::string16& text,
    const gfx::Range& replacement_range) {
  render_view_->OnImeConfirmComposition(text, replacement_range, false);
}

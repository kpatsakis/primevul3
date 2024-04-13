void RootWindow::OnLayerAnimationEnded(
    ui::LayerAnimationSequence* animation) {
  OnHostResized(host_->GetBounds().size());
}

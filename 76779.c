void RootWindow::SetTransform(const ui::Transform& transform) {
  Window::SetTransform(transform);

  if (!layer()->GetAnimator()->is_animating())
    OnHostResized(host_->GetBounds().size());
}

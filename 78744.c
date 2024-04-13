void DelegatedFrameHost::OnCompositingStarted(
    ui::Compositor* compositor, base::TimeTicks start_time) {
  last_draw_ended_ = start_time;
}

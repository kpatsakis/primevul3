void WebContentsImpl::ClearPowerSaveBlockers(
    RenderViewHost* render_view_host) {
  STLDeleteValues(&power_save_blockers_[render_view_host]);
  power_save_blockers_.erase(render_view_host);
}

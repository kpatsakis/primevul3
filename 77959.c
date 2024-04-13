  void GetSizeForRenderViewHost(
      content::RenderViewHost* render_view_host,
      gfx::Size* rwhv_create_size,
      gfx::Size* rwhv_commit_size,
      gfx::Size* wcv_commit_size) {
    RenderViewSizes::const_iterator result = render_view_sizes_.end();
    result = render_view_sizes_.find(render_view_host);
    if (result != render_view_sizes_.end()) {
      *rwhv_create_size = result->second.rwhv_create_size;
      *rwhv_commit_size = result->second.rwhv_commit_size;
      *wcv_commit_size = result->second.wcv_commit_size;
    }
  }

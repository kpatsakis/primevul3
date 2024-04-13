webkit::ppapi::PluginInstance* RenderViewImpl::GetBitmapForOptimizedPluginPaint(
    const gfx::Rect& paint_bounds,
    TransportDIB** dib,
    gfx::Rect* location,
    gfx::Rect* clip,
    float* scale_factor) {
  return pepper_helper_->GetBitmapForOptimizedPluginPaint(
      paint_bounds, dib, location, clip, scale_factor);
}

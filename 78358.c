void RenderThreadImpl::OnMemoryPressure(
    base::MemoryPressureListener::MemoryPressureLevel memory_pressure_level) {
  base::allocator::ReleaseFreeMemory();

  if (memory_pressure_level ==
      base::MemoryPressureListener::MEMORY_PRESSURE_CRITICAL) {
    v8::V8::LowMemoryNotification();

    if (webkit_platform_support_) {
      blink::WebImageCache::clear();
    }

    size_t font_cache_limit = SkGraphics::SetFontCacheLimit(0);
    SkGraphics::SetFontCacheLimit(font_cache_limit);
  } else {
    if (!v8::V8::IdleNotification())
      v8::V8::IdleNotification();
  }
}

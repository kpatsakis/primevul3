bool ImageDataNaClBackend::Init(PPB_ImageData_Impl* impl,
                                 PP_ImageDataFormat format,
                                 int width, int height,
                                 bool init_to_zero) {
  skia_bitmap_.setConfig(SkBitmap::kARGB_8888_Config,
                         impl->width(), impl->height());
  PluginDelegate* plugin_delegate = ResourceHelper::GetPluginDelegate(impl);
  if (!plugin_delegate)
    return false;
  shared_memory_.reset(
      plugin_delegate->CreateAnonymousSharedMemory(skia_bitmap_.getSize()));
  return !!shared_memory_.get();
}

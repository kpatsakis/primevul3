bool ImageDataPlatformBackend::Init(PPB_ImageData_Impl* impl,
                                     PP_ImageDataFormat format,
                                     int width, int height,
                                     bool init_to_zero) {
  PluginDelegate* plugin_delegate = ResourceHelper::GetPluginDelegate(impl);
  if (!plugin_delegate)
    return false;

  platform_image_.reset(plugin_delegate->CreateImage2D(width, height));
  return !!platform_image_.get();
}

WebMediaPlayer* RenderFrameImpl::CreateAndroidWebMediaPlayer(
      const blink::WebURL& url,
      WebMediaPlayerClient* client) {
  GpuChannelHost* gpu_channel_host =
      RenderThreadImpl::current()->EstablishGpuChannelSync(
          CAUSE_FOR_GPU_LAUNCH_VIDEODECODEACCELERATOR_INITIALIZE);
  if (!gpu_channel_host) {
    LOG(ERROR) << "Failed to establish GPU channel for media player";
    return NULL;
  }

  scoped_refptr<StreamTextureFactory> stream_texture_factory;
  if (GetRenderWidget()->UsingSynchronousRendererCompositor()) {
    SynchronousCompositorFactory* factory =
        SynchronousCompositorFactory::GetInstance();
    stream_texture_factory = factory->CreateStreamTextureFactory(
        render_view_->routing_id_);
  } else {
    scoped_refptr<webkit::gpu::ContextProviderWebContext> context_provider =
        RenderThreadImpl::current()->SharedMainThreadContextProvider();

    if (!context_provider.get()) {
      LOG(ERROR) << "Failed to get context3d for media player";
      return NULL;
    }

    stream_texture_factory = StreamTextureFactoryImpl::Create(
        context_provider, gpu_channel_host, render_view_->routing_id_);
  }

  return new WebMediaPlayerAndroid(
      frame_,
      client,
      weak_factory_.GetWeakPtr(),
      render_view_->media_player_manager_,
      stream_texture_factory,
      RenderThreadImpl::current()->GetMediaThreadMessageLoopProxy(),
      new RenderMediaLog());
}

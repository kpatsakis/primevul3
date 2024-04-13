RenderThreadImpl::CreateOffscreenContext3d() {
  blink::WebGraphicsContext3D::Attributes attributes;
  attributes.shareResources = true;
  attributes.depth = false;
  attributes.stencil = false;
  attributes.antialias = false;
  attributes.noAutomaticFlushes = true;
  bool lose_context_when_out_of_memory = true;

  scoped_refptr<GpuChannelHost> gpu_channel_host(EstablishGpuChannelSync(
      CAUSE_FOR_GPU_LAUNCH_WEBGRAPHICSCONTEXT3DCOMMANDBUFFERIMPL_INITIALIZE));
  return make_scoped_ptr(
      WebGraphicsContext3DCommandBufferImpl::CreateOffscreenContext(
          gpu_channel_host.get(),
          attributes,
          lose_context_when_out_of_memory,
          GURL("chrome://gpu/RenderThreadImpl::CreateOffscreenContext3d"),
          WebGraphicsContext3DCommandBufferImpl::SharedMemoryLimits(),
          NULL));
}

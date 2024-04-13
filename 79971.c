WebGraphicsContext3D* RenderViewImpl::CreateGraphicsContext3D(
    const WebGraphicsContext3D::Attributes& attributes) {
  if (!webview())
    return NULL;

  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kInProcessWebGL)) {
    return webkit::gpu::WebGraphicsContext3DInProcessImpl::CreateForWebView(
        attributes, true);
  } else {
    GURL url;
    if (webview()->mainFrame())
      url = GURL(webview()->mainFrame()->document().url());
    else
      url = GURL("chrome://gpu/RenderViewImpl::CreateGraphicsContext3D");

    scoped_ptr<WebGraphicsContext3DCommandBufferImpl> context(
        new WebGraphicsContext3DCommandBufferImpl(
            surface_id(),
            url,
            RenderThreadImpl::current(),
            AsWeakPtr()));

    if (!context->Initialize(
            attributes,
            false /* bind generates resources */,
            CAUSE_FOR_GPU_LAUNCH_WEBGRAPHICSCONTEXT3DCOMMANDBUFFERIMPL_INITIALIZE))
      return NULL;
    return context.release();
  }
}

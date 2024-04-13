void CreateRenderFrameSetup(mojo::InterfaceRequest<RenderFrameSetup> request) {
  mojo::BindToRequest(new RenderFrameSetupImpl(), &request);
}

scoped_ptr<cc::OutputSurface> RenderViewImpl::CreateOutputSurface() {
  WebKit::WebGraphicsContext3D::Attributes attributes;
  attributes.antialias = false;
  attributes.shareResources = true;
  attributes.noAutomaticFlushes = true;
  WebGraphicsContext3D* context = CreateGraphicsContext3D(attributes);
  if (!context)
    return scoped_ptr<cc::OutputSurface>();

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kEnableSoftwareCompositingGLAdapter)) {
      return scoped_ptr<cc::OutputSurface>(
          new CompositorOutputSurface(routing_id(), NULL,
              new CompositorSoftwareOutputDeviceGLAdapter(context)));
  } else {
      bool composite_to_mailbox =
          command_line.HasSwitch(cc::switches::kCompositeToMailbox);
      DCHECK(!composite_to_mailbox || command_line.HasSwitch(
          cc::switches::kEnableCompositorFrameMessage));
      DCHECK(!composite_to_mailbox || is_threaded_compositing_enabled_);
      return scoped_ptr<cc::OutputSurface>(composite_to_mailbox ?
          new MailboxOutputSurface(routing_id(), context, NULL) :
              new CompositorOutputSurface(routing_id(), context, NULL));
  }
}

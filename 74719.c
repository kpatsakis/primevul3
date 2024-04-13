void RenderView::AcceleratedSurfaceSetTransportDIB(
    gfx::PluginWindowHandle window,
    int32 width,
    int32 height,
    TransportDIB::Handle transport_dib) {
  Send(new ViewHostMsg_AcceleratedSurfaceSetTransportDIB(
      routing_id(), window, width, height, transport_dib));
}

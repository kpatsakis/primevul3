void RenderView::AcceleratedSurfaceFreeTransportDIB(TransportDIB::Id dib_id) {
  Send(new ViewHostMsg_FreeTransportDIB(dib_id));
}

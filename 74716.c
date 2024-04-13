TransportDIB::Handle RenderView::AcceleratedSurfaceAllocTransportDIB(
    size_t size) {
  TransportDIB::Handle dib_handle;
  if (Send(new ViewHostMsg_AllocTransportDIB(size, true, &dib_handle)))
    return dib_handle;
  return TransportDIB::DefaultHandleValue();
}

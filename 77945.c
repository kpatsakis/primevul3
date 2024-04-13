void RenderFrameImpl::willOpenSocketStream(
    blink::WebSocketStreamHandle* handle) {
  WebSocketStreamHandleImpl* impl =
      static_cast<WebSocketStreamHandleImpl*>(handle);
  impl->SetUserData(handle, new SocketStreamHandleData(routing_id_));
}

void RenderView::OnHandleMessageFromExternalHost(const std::string& message,
                                                 const std::string& origin,
                                                 const std::string& target) {
  if (message.empty())
    return;
  external_host_bindings_.ForwardMessageFromExternalHost(message, origin,
                                                         target);
}

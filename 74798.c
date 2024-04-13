void RenderView::OnExtensionResponse(int request_id,
                                     bool success,
                                     const std::string& response,
                                     const std::string& error) {
  ExtensionProcessBindings::HandleResponse(
      request_id, success, response, error);
}

void WebContentsImpl::OnPpapiBrokerPermissionResult(int routing_id,
                                                    bool result) {
  Send(new ViewMsg_PpapiBrokerPermissionResult(routing_id, result));
}

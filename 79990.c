SSLStatus RenderViewImpl::GetSSLStatusOfFrame(WebKit::WebFrame* frame) const {
  SSLStatus ssl_status;

  DocumentState* doc_state = DocumentState::FromDataSource(frame->dataSource());
  if (doc_state && !doc_state->security_info().empty()) {
    DeserializeSecurityInfo(doc_state->security_info(),
                            &ssl_status.cert_id,
                            &ssl_status.cert_status,
                            &ssl_status.security_bits,
                            &ssl_status.connection_status);
  }

  return ssl_status;
}

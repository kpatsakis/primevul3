void RenderView::EnsureDocumentTag() {
#if defined(OS_MACOSX)
  if (!has_document_tag_) {
    Send(new ViewHostMsg_GetDocumentTag(routing_id_, &document_tag_));
    has_document_tag_ = true;
  }
#endif
}

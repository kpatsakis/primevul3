bool PrintWebViewHelper::FinalizePrintReadyDocument() {
  DCHECK(!is_print_ready_metafile_sent_);
  print_preview_context_.FinalizePrintReadyDocument();

  printing::PreviewMetafile* metafile = print_preview_context_.metafile();
  uint32 buf_size = metafile->GetDataSize();
  DCHECK_GT(buf_size, 0u);

  PrintHostMsg_DidPreviewDocument_Params preview_params;
  preview_params.reuse_existing_data = false;
  preview_params.data_size = buf_size;
  preview_params.document_cookie = print_pages_params_->params.document_cookie;
  preview_params.expected_pages_count =
      print_preview_context_.total_page_count();
  preview_params.modifiable = print_preview_context_.IsModifiable();
  preview_params.preview_request_id =
      print_pages_params_->params.preview_request_id;

  if (!CopyMetafileDataToSharedMem(metafile,
                                   &(preview_params.metafile_data_handle))) {
    LOG(ERROR) << "CopyMetafileDataToSharedMem failed";
    print_preview_context_.set_error(PREVIEW_ERROR_METAFILE_COPY_FAILED);
    return false;
  }
  is_print_ready_metafile_sent_ = true;

  Send(new PrintHostMsg_MetafileReadyForPrinting(routing_id(), preview_params));
  return true;
}

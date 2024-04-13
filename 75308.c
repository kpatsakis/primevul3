bool PrintWebViewHelper::PrintPreviewContext::IsLastPageOfPrintReadyMetafile()
    const {
  DCHECK(IsRendering());
  return current_page_index_ == print_ready_metafile_page_count_;
}

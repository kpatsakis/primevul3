PrintWebViewHelper::PrintPreviewContext::PrintPreviewContext()
    : frame_(NULL),
      total_page_count_(0),
      current_page_index_(0),
      generate_draft_pages_(true),
      print_ready_metafile_page_count_(0),
      error_(PREVIEW_ERROR_NONE),
      state_(UNINITIALIZED) {
}

RenderView::~RenderView() {
  if (decrement_shared_popup_at_destruction_)
    shared_popup_counter_->data--;

  for (ImageResourceFetcherSet::iterator i = image_fetchers_.begin();
       i != image_fetchers_.end(); ++i) {
    delete *i;
  }

  while (!file_chooser_completions_.empty()) {
    if (file_chooser_completions_.front()->completion) {
      file_chooser_completions_.front()->completion->didChooseFile(
          WebVector<WebString>());
    }
    file_chooser_completions_.pop_front();
  }

#if defined(OS_MACOSX)
  if (has_document_tag_)
    Send(new ViewHostMsg_DocumentWithTagClosed(routing_id_, document_tag_));
#endif

  render_thread_->RemoveFilter(audio_message_filter_);

#ifndef NDEBUG
  ViewMap* views = Singleton<ViewMap>::get();
  for (ViewMap::iterator it = views->begin(); it != views->end(); ++it)
    DCHECK_NE(this, it->second) << "Failed to call Close?";
#endif
}

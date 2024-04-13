void RenderFrameImpl::OnCopyToFindPboard() {
  if (frame_->hasSelection()) {
    base::string16 selection = frame_->selectionAsText();
    RenderThread::Get()->Send(
        new ClipboardHostMsg_FindPboardWriteStringAsync(selection));
  }
}

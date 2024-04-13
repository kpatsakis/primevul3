void WebContentsImpl::OnFrameDetached(int64 parent_frame_id, int64 frame_id) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    FrameDetached(message_source_, frame_id));

  FrameTreeNode* parent = FindFrameTreeNodeByID(parent_frame_id);
  if (!parent)
    return;

  parent->RemoveChild(frame_id);
}

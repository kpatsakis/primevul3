void WebContentsImpl::OnFrameAttached(
    int64 parent_frame_id,
    int64 frame_id,
    const std::string& frame_name) {
  FrameTreeNode* parent = FindFrameTreeNodeByID(parent_frame_id);
  if (!parent)
    return;

  FrameTreeNode* node = new FrameTreeNode(frame_id, frame_name);
  parent->AddChild(node);
}

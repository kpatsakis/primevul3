bool RenderView::GetAllChildFrames(
    WebFrame* parent_frame,
    std::vector<WebFrame*>* frames_vector) const {
  if (!parent_frame)
    return false;
  for (WebFrame* child_frame = parent_frame->firstChild(); child_frame;
       child_frame = child_frame->nextSibling()) {
    frames_vector->push_back(child_frame);
    GetAllChildFrames(child_frame, frames_vector);
  }
  return true;
}

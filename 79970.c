void RenderViewImpl::CreateFrameTree(WebKit::WebFrame* frame,
                                     base::DictionaryValue* frame_tree) {
  DCHECK(false);
  NavigateToSwappedOutURL(frame);

  string16 name;
  if (frame_tree->GetString(kFrameTreeNodeNameKey, &name) && !name.empty())
    frame->setName(name);

  int remote_id;
  if (frame_tree->GetInteger(kFrameTreeNodeIdKey, &remote_id))
    active_frame_id_map_.insert(std::pair<int, int>(frame->identifier(),
                                                    remote_id));

  base::ListValue* children;
  if (!frame_tree->GetList(kFrameTreeNodeSubtreeKey, &children))
    return;

  base::DictionaryValue* child;
  for (size_t i = 0; i < children->GetSize(); ++i) {
    if (!children->GetDictionary(i, &child))
      continue;
    WebElement element = frame->document().createElement("iframe");
    element.setAttribute("width", "0");
    element.setAttribute("height", "0");
    element.setAttribute("frameBorder", "0");
    if (frame->document().body().appendChild(element)) {
      WebFrame* subframe = WebFrame::fromFrameOwnerElement(element);
      if (subframe)
        CreateFrameTree(subframe, child);
    } else {
      LOG(ERROR) << "Failed to append created iframe element.";
    }
  }
}

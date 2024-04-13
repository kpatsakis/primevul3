void RenderView::logCrossFramePropertyAccess(WebFrame* frame,
                                             WebFrame* target,
                                             bool cross_origin,
                                             const WebString& property_name,
                                             unsigned long long event_id) {
  if (cross_origin)
    cross_origin_access_count_++;
  else
    same_origin_access_count_++;
}

static WebRect elementBounds(WebFrame* frame, const WebString& id)
{
    return frame->document().getElementById(id).boundsInViewportSpace();
}

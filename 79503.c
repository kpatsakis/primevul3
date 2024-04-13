static RenderVideo* findFullscreenVideoRenderer(Document* document)
{
    Element* fullscreenElement = FullscreenElementStack::currentFullScreenElementFrom(document);
    while (fullscreenElement && fullscreenElement->isFrameOwnerElement()) {
        document = toHTMLFrameOwnerElement(fullscreenElement)->contentDocument();
        if (!document)
            return 0;
        fullscreenElement = FullscreenElementStack::currentFullScreenElementFrom(document);
    }
    if (!fullscreenElement || !fullscreenElement->hasTagName(videoTag))
        return 0;
    RenderObject* renderer = fullscreenElement->renderer();
    if (!renderer)
        return 0;
    return toRenderVideo(renderer);
}

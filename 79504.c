void RenderLayerCompositor::finishCompositingUpdateForFrameTree(Frame* frame)
{
    for (Frame* child = frame->tree().firstChild(); child; child = child->tree().nextSibling())
        finishCompositingUpdateForFrameTree(child);

    if (frame && frame->contentRenderer()) {
        RenderLayerCompositor* frameCompositor = frame->contentRenderer()->compositor();
        if (frameCompositor && !frameCompositor->isMainFrame())
            frame->contentRenderer()->compositor()->updateCompositingLayers();
    }
}

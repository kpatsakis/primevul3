static float localZoomForRenderer(RenderObject* renderer)
{
    float zoomFactor = 1;
    if (renderer->style()->effectiveZoom() != 1) {
        RenderObject* prev = renderer;
        for (RenderObject* curr = prev->parent(); curr; curr = curr->parent()) {
            if (curr->style()->effectiveZoom() != prev->style()->effectiveZoom()) {
                zoomFactor = prev->style()->zoom();
                break;
            }
            prev = curr;
        }
        if (prev->isRenderView())
            zoomFactor = prev->style()->zoom();
    }
    return zoomFactor;
}

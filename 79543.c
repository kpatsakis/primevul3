bool RenderLayerCompositor::requiresCompositingForCanvas(RenderObject* renderer) const
{
    if (!(m_compositingTriggers & ChromeClient::CanvasTrigger))
        return false;

    if (renderer->isCanvas()) {
        HTMLCanvasElement* canvas = toHTMLCanvasElement(renderer->node());
        return canvas->renderingContext() && canvas->renderingContext()->isAccelerated();
    }
    return false;
}

bool RenderLayerCompositor::requiresCompositingForFilters(RenderObject* renderer) const
{
    if (!(m_compositingTriggers & ChromeClient::FilterTrigger))
        return false;

    return renderer->hasFilter();
}

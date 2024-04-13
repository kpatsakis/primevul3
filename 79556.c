bool RenderLayerCompositor::requiresOverhangLayers() const
{
    if (!isMainFrame())
        return false;

    if (scrollingCoordinator() && m_renderView->frameView()->hasOpaqueBackground())
        return true;

    return true;
}

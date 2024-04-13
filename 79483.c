void RenderLayerCompositor::cacheAcceleratedCompositingFlags()
{
    bool hasAcceleratedCompositing = false;
    bool showRepaintCounter = false;
    bool forceCompositingMode = false;

    if (Settings* settings = m_renderView->document().settings()) {
        hasAcceleratedCompositing = settings->acceleratedCompositingEnabled();

        if (hasAcceleratedCompositing) {
            if (Page* page = this->page()) {
                m_compositingTriggers = page->chrome().client().allowedCompositingTriggers();
                hasAcceleratedCompositing = m_compositingTriggers;
            }
        }

        showRepaintCounter = settings->showRepaintCounter();
        forceCompositingMode = settings->forceCompositingMode() && hasAcceleratedCompositing;

        if (forceCompositingMode && !isMainFrame())
            forceCompositingMode = requiresCompositingForScrollableFrame();
    }

    if (hasAcceleratedCompositing != m_hasAcceleratedCompositing || showRepaintCounter != m_showRepaintCounter || forceCompositingMode != m_forceCompositingMode)
        setCompositingLayersNeedRebuild();

    m_hasAcceleratedCompositing = hasAcceleratedCompositing;
    m_showRepaintCounter = showRepaintCounter;
    m_forceCompositingMode = forceCompositingMode;
}

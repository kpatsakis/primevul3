bool RenderLayerCompositor::requiresCompositingForTransition(RenderObject* renderer) const
{
    if (!(m_compositingTriggers & ChromeClient::AnimationTrigger))
        return false;

    if (Settings* settings = m_renderView->document().settings()) {
        if (!settings->acceleratedCompositingForTransitionEnabled())
            return false;
    }

    return renderer->style()->transitionForProperty(CSSPropertyOpacity)
        || renderer->style()->transitionForProperty(CSSPropertyWebkitFilter)
        || renderer->style()->transitionForProperty(CSSPropertyWebkitTransform);
}

bool RenderLayerCompositor::requiresCompositingForVideo(RenderObject* renderer) const
{
    if (RuntimeEnabledFeatures::overlayFullscreenVideoEnabled() && renderer->isVideo()) {
        HTMLMediaElement* media = toHTMLMediaElement(renderer->node());
        if (media->isFullscreen())
            return true;
    }

    if (!(m_compositingTriggers & ChromeClient::VideoTrigger))
        return false;

    if (renderer->isVideo()) {
        RenderVideo* video = toRenderVideo(renderer);
        return video->shouldDisplayVideo() && canAccelerateVideoRendering(video);
    }
    return false;
}

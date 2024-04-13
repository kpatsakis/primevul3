void RenderBox::imageChanged(WrappedImagePtr image, const IntRect*)
{
    if (!parent())
        return;

    if ((style()->borderImage().image() && style()->borderImage().image()->data() == image) ||
        (style()->maskBoxImage().image() && style()->maskBoxImage().image()->data() == image)) {
        repaint();
        return;
    }

    bool didFullRepaint = repaintLayerRectsForImage(image, style()->backgroundLayers(), true);
    if (!didFullRepaint)
        repaintLayerRectsForImage(image, style()->maskLayers(), false);


#if USE(ACCELERATED_COMPOSITING)
    if (hasLayer() && layer()->hasCompositedMask() && layersUseImage(image, style()->maskLayers()))
        layer()->contentChanged(RenderLayer::MaskImageChanged);
#endif
}

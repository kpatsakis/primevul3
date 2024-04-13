RenderRegion* Element::renderRegion() const
{
    if (renderer() && renderer()->isRenderRegion())
        return toRenderRegion(renderer());

    return 0;
}

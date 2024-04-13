Vector<RefPtr<Range> > Element::webkitGetRegionFlowRanges() const
{
    document()->updateLayoutIgnorePendingStylesheets();

    Vector<RefPtr<Range> > rangeObjects;
    if (RuntimeEnabledFeatures::cssRegionsEnabled() && renderer() && renderer()->isRenderRegion()) {
        RenderRegion* region = toRenderRegion(renderer());
        if (region->isValid())
            region->getRanges(rangeObjects);
    }

    return rangeObjects;
}

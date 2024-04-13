IntRect RenderBox::outlineBoundsForRepaint(RenderBoxModelObject* repaintContainer, IntPoint* cachedOffsetToRepaintContainer) const
{
    IntRect box = borderBoundingBox();
    adjustRectForOutlineAndShadow(box);

    FloatQuad containerRelativeQuad = FloatRect(box);
    if (cachedOffsetToRepaintContainer)
        containerRelativeQuad.move(cachedOffsetToRepaintContainer->x(), cachedOffsetToRepaintContainer->y());
    else
        containerRelativeQuad = localToContainerQuad(containerRelativeQuad, repaintContainer);

    box = containerRelativeQuad.enclosingBoundingBox();

    box.move(view()->layoutDelta());

    return box;
}

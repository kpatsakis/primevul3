ShapeInsideInfo* RenderBlock::layoutShapeInsideInfo() const
{
    ShapeInsideInfo* shapeInsideInfo = view()->layoutState()->shapeInsideInfo();

    if (!shapeInsideInfo && flowThreadContainingBlock() && allowsShapeInsideInfoSharing()) {
        LayoutUnit offset = logicalHeight() + logicalHeightForLine(this, false) - LayoutUnit(1);
        RenderRegion* region = regionAtBlockOffset(offset);
        if (region)
            shapeInsideInfo = region->shapeInsideInfo();
    }

    return shapeInsideInfo;
}

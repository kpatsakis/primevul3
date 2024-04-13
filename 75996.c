IntRect RenderBox::layoutOverflowRectForPropagation(RenderStyle* parentStyle) const
{
    IntRect rect = borderBoxRect();
    if (!hasOverflowClip())
        rect.unite(layoutOverflowRect());

    bool hasTransform = hasLayer() && layer()->transform();
    if (isRelPositioned() || hasTransform) {
        flipForWritingMode(rect);
        
        if (hasTransform)
            rect = layer()->currentTransform().mapRect(rect);

        if (isRelPositioned())
            rect.move(relativePositionOffsetX(), relativePositionOffsetY());
        
        flipForWritingMode(rect);
    }
    
    if (parentStyle->writingMode() == style()->writingMode())
        return rect;
    
    if (style()->writingMode() == RightToLeftWritingMode || parentStyle->writingMode() == RightToLeftWritingMode)
        rect.setX(width() - rect.maxX());
    else if (style()->writingMode() == BottomToTopWritingMode || parentStyle->writingMode() == BottomToTopWritingMode)
        rect.setY(height() - rect.maxY());

    return rect;
}

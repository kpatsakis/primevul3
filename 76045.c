void RenderBox::styleDidChange(StyleDifference diff, const RenderStyle* oldStyle)
{
    RenderBoxModelObject::styleDidChange(diff, oldStyle);

    if (needsLayout() && oldStyle) {
        if (oldStyle && (oldStyle->logicalHeight().isPercent() || oldStyle->logicalMinHeight().isPercent() || oldStyle->logicalMaxHeight().isPercent()))
            RenderBlock::removePercentHeightDescendant(this);

        if (isPositioned() && style()->hasStaticBlockPosition(isHorizontalWritingMode()) && oldStyle->marginBefore() != style()->marginBefore()
            && parent() && !parent()->normalChildNeedsLayout())
            parent()->setChildNeedsLayout(true);
    }

    if (hasOverflowClip() && oldStyle && style() && oldStyle->effectiveZoom() != style()->effectiveZoom()) {
        if (int left = layer()->scrollXOffset()) {
            left = (left / oldStyle->effectiveZoom()) * style()->effectiveZoom();
            layer()->scrollToXOffset(left);
        }
        if (int top = layer()->scrollYOffset()) {
            top = (top / oldStyle->effectiveZoom()) * style()->effectiveZoom();
            layer()->scrollToYOffset(top);
        }
    }

    bool isBodyRenderer = isBody();
    bool isRootRenderer = isRoot();

    if (isBodyRenderer)
        document()->setTextColor(style()->visitedDependentColor(CSSPropertyColor));

    if (isRootRenderer || isBodyRenderer) {
        RenderView* viewRenderer = view();
        RenderStyle* viewStyle = viewRenderer->style();
        if (viewStyle->direction() != style()->direction() && (isRootRenderer || !document()->directionSetOnDocumentElement())) {
            viewStyle->setDirection(style()->direction());
            if (isBodyRenderer)
                document()->documentElement()->renderer()->style()->setDirection(style()->direction());
            setNeedsLayoutAndPrefWidthsRecalc();
        }

        if (viewStyle->writingMode() != style()->writingMode() && (isRootRenderer || !document()->writingModeSetOnDocumentElement())) {
            viewStyle->setWritingMode(style()->writingMode());
            viewRenderer->setHorizontalWritingMode(style()->isHorizontalWritingMode());
            if (isBodyRenderer) {
                document()->documentElement()->renderer()->style()->setWritingMode(style()->writingMode());
                document()->documentElement()->renderer()->setHorizontalWritingMode(style()->isHorizontalWritingMode());
            }
            setNeedsLayoutAndPrefWidthsRecalc();
        }

        frame()->view()->recalculateScrollbarOverlayStyle();
    }
}

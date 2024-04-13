void RenderBlock::layoutInlineChildren(bool relayoutChildren, LayoutUnit& repaintLogicalTop, LayoutUnit& repaintLogicalBottom)
{
    setLogicalHeight(borderBefore() + paddingBefore());

    if (view()->layoutState() && view()->layoutState()->lineGrid() == this)
        layoutLineGridBox();

    RenderFlowThread* flowThread = flowThreadContainingBlock();
    bool clearLinesForPagination = firstLineBox() && flowThread && !flowThread->hasRegions();

    bool isFullLayout = !firstLineBox() || selfNeedsLayout() || relayoutChildren || clearLinesForPagination;
    LineLayoutState layoutState(isFullLayout, repaintLogicalTop, repaintLogicalBottom, flowThread);

    if (isFullLayout)
        lineBoxes()->deleteLineBoxes();

    bool hasTextOverflow = (style()->textOverflow() && hasOverflowClip())
        || (isAnonymousBlock() && parent() && parent()->isRenderBlock() && parent()->style()->textOverflow() && parent()->hasOverflowClip());

    if (hasTextOverflow)
         deleteEllipsisLineBoxes();

    if (firstChild()) {
        bool hasInlineChild = false;
        Vector<RenderBox*> replacedChildren;
        for (InlineWalker walker(this); !walker.atEnd(); walker.advance()) {
            RenderObject* o = walker.current();
            if (!hasInlineChild && o->isInline())
                hasInlineChild = true;

            if (o->isReplaced() || o->isFloating() || o->isOutOfFlowPositioned()) {
                RenderBox* box = toRenderBox(o);

                if (relayoutChildren || box->hasRelativeDimensions())
                    o->setChildNeedsLayout(MarkOnlyThis);

                if (relayoutChildren && box->needsPreferredWidthsRecalculation())
                    o->setPreferredLogicalWidthsDirty(MarkOnlyThis);

                if (o->isOutOfFlowPositioned())
                    o->containingBlock()->insertPositionedObject(box);
                else if (o->isFloating())
                    layoutState.floats().append(FloatWithRect(box));
                else if (isFullLayout || o->needsLayout()) {
                    box->dirtyLineBoxes(isFullLayout);
                    if (isFullLayout)
                        replacedChildren.append(box);
                    else
                        o->layoutIfNeeded();
                }
            } else if (o->isText() || (o->isRenderInline() && !walker.atEndOfInline())) {
                if (!o->isText())
                    toRenderInline(o)->updateAlwaysCreateLineBoxes(layoutState.isFullLayout());
                if (layoutState.isFullLayout() || o->selfNeedsLayout())
                    dirtyLineBoxesForRenderer(o, layoutState.isFullLayout());
                o->clearNeedsLayout();
            }
        }

        for (size_t i = 0; i < replacedChildren.size(); i++)
            replacedChildren[i]->layoutIfNeeded();

        layoutRunsAndFloats(layoutState, hasInlineChild);
    }

    int lastLineAnnotationsAdjustment = 0;
    if (lastRootBox()) {
        LayoutUnit lowestAllowedPosition = max(lastRootBox()->lineBottom(), logicalHeight() + paddingAfter());
        if (!style()->isFlippedLinesWritingMode())
            lastLineAnnotationsAdjustment = lastRootBox()->computeUnderAnnotationAdjustment(lowestAllowedPosition);
        else
            lastLineAnnotationsAdjustment = lastRootBox()->computeOverAnnotationAdjustment(lowestAllowedPosition);
    }

    setLogicalHeight(logicalHeight() + lastLineAnnotationsAdjustment + borderAfter() + paddingAfter() + scrollbarLogicalHeight());

    if (!firstLineBox() && hasLineIfEmpty())
        setLogicalHeight(logicalHeight() + lineHeight(true, isHorizontalWritingMode() ? HorizontalLine : VerticalLine, PositionOfInteriorLineBoxes));

    if (hasTextOverflow)
        checkLinesForTextOverflow();
}

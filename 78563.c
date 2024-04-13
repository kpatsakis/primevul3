bool RenderBlock::positionNewFloatOnLine(FloatingObject* newFloat, FloatingObject* lastFloatFromPreviousLine, LineInfo& lineInfo, LineWidth& width)
{
    if (!positionNewFloats())
        return false;

    width.shrinkAvailableWidthForNewFloatIfNeeded(newFloat);

    if (!newFloat->paginationStrut() || !lineInfo.previousLineBrokeCleanly() || !lineInfo.isEmpty())
        return true;

    const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
    ASSERT(floatingObjectSet.last() == newFloat);

    LayoutUnit floatLogicalTop = newFloat->logicalTop(isHorizontalWritingMode());
    int paginationStrut = newFloat->paginationStrut();

    if (floatLogicalTop - paginationStrut != logicalHeight() + lineInfo.floatPaginationStrut())
        return true;

    FloatingObjectSetIterator it = floatingObjectSet.end();
    --it; // Last float is newFloat, skip that one.
    FloatingObjectSetIterator begin = floatingObjectSet.begin();
    while (it != begin) {
        --it;
        FloatingObject* f = *it;
        if (f == lastFloatFromPreviousLine)
            break;
        if (f->logicalTop(isHorizontalWritingMode()) == logicalHeight() + lineInfo.floatPaginationStrut()) {
            f->setPaginationStrut(paginationStrut + f->paginationStrut());
            RenderBox* o = f->renderer();
            setLogicalTopForChild(o, logicalTopForChild(o) + marginBeforeForChild(o) + paginationStrut);
            if (o->isRenderBlock())
                o->forceChildLayout();
            else
                o->layoutIfNeeded();
            LayoutUnit oldLogicalTop = f->logicalTop(isHorizontalWritingMode());
            m_floatingObjects->removePlacedObject(f);
            f->setLogicalTop(oldLogicalTop + paginationStrut, isHorizontalWritingMode());
            m_floatingObjects->addPlacedObject(f);
        }
    }

    lineInfo.setFloatPaginationStrut(lineInfo.floatPaginationStrut() + paginationStrut);
    return true;
}

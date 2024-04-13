bool InlineFlowBox::canAccommodateEllipsis(bool ltr, int blockEdge, int ellipsisWidth)
{
    for (InlineBox *box = firstChild(); box; box = box->nextOnLine()) {
        if (!box->canAccommodateEllipsis(ltr, blockEdge, ellipsisWidth))
            return false;
    }
    return true;
}

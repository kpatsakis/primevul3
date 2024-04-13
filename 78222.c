void ApplyBlockElementCommand::doApply()
{
    if (!endingSelection().rootEditableElement())
        return;

    VisiblePosition visibleEnd = endingSelection().visibleEnd();
    VisiblePosition visibleStart = endingSelection().visibleStart();
    if (visibleStart.isNull() || visibleStart.isOrphan() || visibleEnd.isNull() || visibleEnd.isOrphan())
        return;

    if (visibleEnd != visibleStart && isStartOfParagraph(visibleEnd))
        setEndingSelection(VisibleSelection(visibleStart, visibleEnd.previous(CannotCrossEditingBoundary), endingSelection().isDirectional()));

    VisibleSelection selection = selectionForParagraphIteration(endingSelection());
    VisiblePosition startOfSelection = selection.visibleStart();
    VisiblePosition endOfSelection = selection.visibleEnd();
    ASSERT(!startOfSelection.isNull());
    ASSERT(!endOfSelection.isNull());
    RefPtr<ContainerNode> startScope;
    int startIndex = indexForVisiblePosition(startOfSelection, startScope);
    RefPtr<ContainerNode> endScope;
    int endIndex = indexForVisiblePosition(endOfSelection, endScope);

    formatSelection(startOfSelection, endOfSelection);

    document().updateLayoutIgnorePendingStylesheets();

    ASSERT(startScope == endScope);
    ASSERT(startIndex >= 0);
    ASSERT(startIndex <= endIndex);
    if (startScope == endScope && startIndex >= 0 && startIndex <= endIndex) {
        VisiblePosition start(visiblePositionForIndex(startIndex, startScope.get()));
        VisiblePosition end(visiblePositionForIndex(endIndex, endScope.get()));
        if (start.isNotNull() && end.isNotNull())
            setEndingSelection(VisibleSelection(start, end, endingSelection().isDirectional()));
    }
}

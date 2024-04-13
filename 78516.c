static void deleteLineRange(LineLayoutState& layoutState, RootInlineBox* startLine, RootInlineBox* stopLine = 0)
{
    RootInlineBox* boxToDelete = startLine;
    while (boxToDelete && boxToDelete != stopLine) {
        layoutState.updateRepaintRangeFromBox(boxToDelete);
        RootInlineBox* next = boxToDelete->nextRootBox();
        boxToDelete->deleteLine();
        boxToDelete = next;
    }
}

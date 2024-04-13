void InlineFlowBox::addBoxShadowVisualOverflow(IntRect& logicalVisualOverflow)
{
    if (!parent())
        return; // Box-shadow doesn't apply to root line boxes.

    int boxShadowLogicalTop;
    int boxShadowLogicalBottom;
    renderer()->style(m_firstLine)->getBoxShadowBlockDirectionExtent(boxShadowLogicalTop, boxShadowLogicalBottom);
    
    int logicalTopVisualOverflow = min(logicalTop() + boxShadowLogicalTop, logicalVisualOverflow.y());
    int logicalBottomVisualOverflow = max(logicalBottom() + boxShadowLogicalBottom, logicalVisualOverflow.maxY());
    
    int boxShadowLogicalLeft;
    int boxShadowLogicalRight;
    renderer()->style(m_firstLine)->getBoxShadowInlineDirectionExtent(boxShadowLogicalLeft, boxShadowLogicalRight);

    int logicalLeftVisualOverflow = min(pixelSnappedLogicalLeft() + boxShadowLogicalLeft, logicalVisualOverflow.x());
    int logicalRightVisualOverflow = max(pixelSnappedLogicalRight() + boxShadowLogicalRight, logicalVisualOverflow.maxX());
    
    logicalVisualOverflow = IntRect(logicalLeftVisualOverflow, logicalTopVisualOverflow,
                                    logicalRightVisualOverflow - logicalLeftVisualOverflow, logicalBottomVisualOverflow - logicalTopVisualOverflow);
}

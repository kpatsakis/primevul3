void ApplyBlockElementCommand::rangeForParagraphSplittingTextNodesIfNeeded(const VisiblePosition& endOfCurrentParagraph, Position& start, Position& end)
{
    start = startOfParagraph(endOfCurrentParagraph).deepEquivalent();
    end = endOfCurrentParagraph.deepEquivalent();

    document().updateStyleIfNeeded();

    bool isStartAndEndOnSameNode = false;
    if (RenderStyle* startStyle = renderStyleOfEnclosingTextNode(start)) {
        isStartAndEndOnSameNode = renderStyleOfEnclosingTextNode(end) && start.containerNode() == end.containerNode();
        bool isStartAndEndOfLastParagraphOnSameNode = renderStyleOfEnclosingTextNode(m_endOfLastParagraph) && start.containerNode() == m_endOfLastParagraph.containerNode();

        if (startStyle->preserveNewline() && isNewLineAtPosition(start) && !isNewLineAtPosition(start.previous()) && start.offsetInContainerNode() > 0)
            start = startOfParagraph(end.previous()).deepEquivalent();

        if (!startStyle->collapseWhiteSpace() && start.offsetInContainerNode() > 0) {
            int startOffset = start.offsetInContainerNode();
            Text* startText = start.containerText();
            splitTextNode(startText, startOffset);
            start = firstPositionInNode(startText);
            if (isStartAndEndOnSameNode) {
                ASSERT(end.offsetInContainerNode() >= startOffset);
                end = Position(startText, end.offsetInContainerNode() - startOffset);
            }
            if (isStartAndEndOfLastParagraphOnSameNode) {
                ASSERT(m_endOfLastParagraph.offsetInContainerNode() >= startOffset);
                m_endOfLastParagraph = Position(startText, m_endOfLastParagraph.offsetInContainerNode() - startOffset);
            }
        }
    }

    document().updateStyleIfNeeded();

    if (RenderStyle* endStyle = renderStyleOfEnclosingTextNode(end)) {
        bool isEndAndEndOfLastParagraphOnSameNode = renderStyleOfEnclosingTextNode(m_endOfLastParagraph) && end.deprecatedNode() == m_endOfLastParagraph.deprecatedNode();
        if (endStyle->preserveNewline() && start == end && end.offsetInContainerNode() < end.containerNode()->maxCharacterOffset()) {
            int endOffset = end.offsetInContainerNode();
            if (!isNewLineAtPosition(end.previous()) && isNewLineAtPosition(end))
                end = Position(end.containerText(), endOffset + 1);
            if (isEndAndEndOfLastParagraphOnSameNode && end.offsetInContainerNode() >= m_endOfLastParagraph.offsetInContainerNode())
                m_endOfLastParagraph = end;
        }

        if (!endStyle->collapseWhiteSpace() && end.offsetInContainerNode() && end.offsetInContainerNode() < end.containerNode()->maxCharacterOffset()) {
            RefPtr<Text> endContainer = end.containerText();
            splitTextNode(endContainer, end.offsetInContainerNode());
            if (isStartAndEndOnSameNode)
                start = firstPositionInOrBeforeNode(endContainer->previousSibling());
            if (isEndAndEndOfLastParagraphOnSameNode) {
                if (m_endOfLastParagraph.offsetInContainerNode() == end.offsetInContainerNode())
                    m_endOfLastParagraph = lastPositionInOrAfterNode(endContainer->previousSibling());
                else
                    m_endOfLastParagraph = Position(endContainer, m_endOfLastParagraph.offsetInContainerNode() - end.offsetInContainerNode());
            }
            end = lastPositionInNode(endContainer->previousSibling());
        }
    }
}

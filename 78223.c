VisiblePosition ApplyBlockElementCommand::endOfNextParagrahSplittingTextNodesIfNeeded(VisiblePosition& endOfCurrentParagraph, Position& start, Position& end)
{
    VisiblePosition endOfNextParagraph = endOfParagraph(endOfCurrentParagraph.next());
    Position position = endOfNextParagraph.deepEquivalent();
    RenderStyle* style = renderStyleOfEnclosingTextNode(position);
    if (!style)
        return endOfNextParagraph;

    RefPtr<Text> text = position.containerText();
    if (!style->preserveNewline() || !position.offsetInContainerNode() || !isNewLineAtPosition(firstPositionInNode(text.get())))
        return endOfNextParagraph;

    splitTextNode(text, 1);

    if (text == start.containerNode() && text->previousSibling() && text->previousSibling()->isTextNode()) {
        ASSERT(start.offsetInContainerNode() < position.offsetInContainerNode());
        start = Position(toText(text->previousSibling()), start.offsetInContainerNode());
    }
    if (text == end.containerNode() && text->previousSibling() && text->previousSibling()->isTextNode()) {
        ASSERT(end.offsetInContainerNode() < position.offsetInContainerNode());
        end = Position(toText(text->previousSibling()), end.offsetInContainerNode());
    }
    if (text == m_endOfLastParagraph.containerNode()) {
        if (m_endOfLastParagraph.offsetInContainerNode() < position.offsetInContainerNode()) {
            if (text->previousSibling()->isTextNode()
                && static_cast<unsigned>(m_endOfLastParagraph.offsetInContainerNode()) <= toText(text->previousSibling())->length())
                m_endOfLastParagraph = Position(toText(text->previousSibling()), m_endOfLastParagraph.offsetInContainerNode());
        } else
            m_endOfLastParagraph = Position(text.get(), m_endOfLastParagraph.offsetInContainerNode() - 1);
    }

    return Position(text.get(), position.offsetInContainerNode() - 1);
}

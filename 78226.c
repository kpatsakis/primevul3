static RenderStyle* renderStyleOfEnclosingTextNode(const Position& position)
{
    if (position.anchorType() != Position::PositionIsOffsetInAnchor || !position.containerNode() || !position.containerNode()->isTextNode())
        return 0;
    return position.containerNode()->renderStyle();
}

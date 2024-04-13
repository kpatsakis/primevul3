bool TextAutosizer::clusterShouldBeAutosized(const RenderBlock* lowestCommonAncestor, float commonAncestorWidth)
{
    const float minLinesOfText = 4;
    float minTextWidth = commonAncestorWidth * minLinesOfText;
    float textWidth = 0;
    measureDescendantTextWidth(lowestCommonAncestor, minTextWidth, textWidth);
    if (textWidth >= minTextWidth)
        return true;
    return false;
}

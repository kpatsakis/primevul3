bool RenderBox::scroll(ScrollDirection direction, ScrollGranularity granularity, float multiplier, Node** stopNode)
{
    RenderLayer* l = layer();
    if (l && l->scroll(direction, granularity, multiplier)) {
        if (stopNode)
            *stopNode = node();
        return true;
    }

    if (stopNode && *stopNode && *stopNode == node())
        return true;

    RenderBlock* b = containingBlock();
    if (b && !b->isRenderView())
        return b->scroll(direction, granularity, multiplier, stopNode);
    return false;
}

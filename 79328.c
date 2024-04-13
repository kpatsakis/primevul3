static int maxOffsetIncludingCollapsedSpaces(Node* node)
{
    int offset = caretMaxOffset(node);

    if (node->renderer() && node->renderer()->isText())
        offset += collapsedSpaceLength(toRenderText(node->renderer()), offset);

    return offset;
}

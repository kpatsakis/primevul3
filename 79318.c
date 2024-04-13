static inline bool ignoresContainerClip(Node* node)
{
    RenderObject* renderer = node->renderer();
    if (!renderer || renderer->isText())
        return false;
    return renderer->style()->hasOutOfFlowPosition();
}

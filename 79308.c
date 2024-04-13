static inline bool fullyClipsContents(Node* node)
{
    RenderObject* renderer = node->renderer();
    if (!renderer || !renderer->isBox() || !renderer->hasOverflowClip())
        return false;
    return toRenderBox(renderer)->size().isEmpty();
}

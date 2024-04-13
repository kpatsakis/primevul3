bool TextAutosizer::isAutosizingContainer(const RenderObject* renderer)
{
    return renderer->isRenderBlock() && !renderer->isInline() && !renderer->isListItem();
}

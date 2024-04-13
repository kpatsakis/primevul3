void TextAutosizer::processContainer(float multiplier, RenderBlock* container, RenderObject* subtreeRoot, const TextAutosizingWindowInfo& windowInfo)
{
    ASSERT(isAutosizingContainer(container));

    float localMultiplier = contentHeightIsConstrained(container) ? 1 : multiplier;

    RenderObject* descendant = nextInPreOrderSkippingDescendantsOfContainers(subtreeRoot, subtreeRoot);
    while (descendant) {
        if (descendant->isText()) {
            if (localMultiplier != descendant->style()->textAutosizingMultiplier()) {
                setMultiplier(descendant, localMultiplier);
                setMultiplier(descendant->parent(), localMultiplier); // Parent does line spacing.
            }
        } else if (isAutosizingContainer(descendant)) {
            RenderBlock* descendantBlock = toRenderBlock(descendant);
            if (isAutosizingCluster(descendantBlock))
                processCluster(descendantBlock, descendantBlock, descendantBlock, windowInfo);
            else
                processContainer(multiplier, descendantBlock, descendantBlock, windowInfo);
        }
        descendant = nextInPreOrderSkippingDescendantsOfContainers(descendant, subtreeRoot);
    }
}

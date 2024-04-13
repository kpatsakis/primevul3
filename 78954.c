bool Element::rendererIsNeeded(const NodeRenderingContext& context)
{
    return context.style()->display() != NONE;
}

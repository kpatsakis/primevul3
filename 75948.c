bool RenderBox::canBeProgramaticallyScrolled() const
{
    return (hasOverflowClip() && (scrollsOverflow() || (node() && node()->rendererIsEditable()))) || (node() && node()->isDocumentNode());
}

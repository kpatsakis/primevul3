Element* Element::offsetParent()
{
    document()->updateLayoutIgnorePendingStylesheets();
    if (RenderObject* renderer = this->renderer())
        return renderer->offsetParent();
    return 0;
}

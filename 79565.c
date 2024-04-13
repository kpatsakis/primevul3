ScrollingCoordinator* RenderLayerCompositor::scrollingCoordinator() const
{
    if (Page* page = this->page())
        return page->scrollingCoordinator();

    return 0;
}

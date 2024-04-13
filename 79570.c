static bool shouldCompositeOverflowControls(FrameView* view)
{
    if (Page* page = view->frame().page()) {
        if (ScrollingCoordinator* scrollingCoordinator = page->scrollingCoordinator())
            if (scrollingCoordinator->coordinatesScrollingForFrameView(view))
                return true;
    }

    return true;
}

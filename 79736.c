bool HTMLSelectElement::platformHandleKeydownEvent(KeyboardEvent* event)
{
    if (!RenderTheme::theme().popsMenuByArrowKeys())
        return false;

    if (!isSpatialNavigationEnabled(document().frame())) {
        if (event->keyIdentifier() == "Down" || event->keyIdentifier() == "Up") {
            focus();
            if (!renderer())
                return true;

            saveLastSelection();
            if (RenderMenuList* menuList = toRenderMenuList(renderer()))
                menuList->showPopup();
            event->setDefaultHandled();
        }
        return true;
    }

    return false;
}

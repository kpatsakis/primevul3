void HTMLSelectElement::menuListDefaultEventHandler(Event* event)
{
    RenderTheme& renderTheme = RenderTheme::theme();

    if (event->type() == EventTypeNames::keydown) {
        if (!renderer() || !event->isKeyboardEvent())
            return;

        if (platformHandleKeydownEvent(toKeyboardEvent(event)))
            return;

        if (isSpatialNavigationEnabled(document().frame())) {
            if (!m_activeSelectionState)
                return;
        }

        const String& keyIdentifier = toKeyboardEvent(event)->keyIdentifier();
        bool handled = true;
        const Vector<HTMLElement*>& listItems = this->listItems();
        int listIndex = optionToListIndex(selectedIndex());

        if (keyIdentifier == "Down" || keyIdentifier == "Right")
            listIndex = nextValidIndex(listIndex, SkipForwards, 1);
        else if (keyIdentifier == "Up" || keyIdentifier == "Left")
            listIndex = nextValidIndex(listIndex, SkipBackwards, 1);
        else if (keyIdentifier == "PageDown")
            listIndex = nextValidIndex(listIndex, SkipForwards, 3);
        else if (keyIdentifier == "PageUp")
            listIndex = nextValidIndex(listIndex, SkipBackwards, 3);
        else if (keyIdentifier == "Home")
            listIndex = nextValidIndex(-1, SkipForwards, 1);
        else if (keyIdentifier == "End")
            listIndex = nextValidIndex(listItems.size(), SkipBackwards, 1);
        else
            handled = false;

        if (handled && static_cast<size_t>(listIndex) < listItems.size())
            selectOption(listToOptionIndex(listIndex), DeselectOtherOptions | DispatchChangeEvent | UserDriven);

        if (handled)
            event->setDefaultHandled();
    }

    if (event->type() == EventTypeNames::keypress) {
        if (!renderer() || !event->isKeyboardEvent())
            return;

        int keyCode = toKeyboardEvent(event)->keyCode();
        bool handled = false;

        if (keyCode == ' ' && isSpatialNavigationEnabled(document().frame())) {
            m_activeSelectionState = !m_activeSelectionState;
            event->setDefaultHandled();
            return;
        }

        if (renderTheme.popsMenuBySpaceOrReturn()) {
            if (keyCode == ' ' || keyCode == '\r') {
                focus();

                if (!renderer() || !renderer()->isMenuList())
                    return;

                saveLastSelection();
                if (RenderMenuList* menuList = toRenderMenuList(renderer()))
                    menuList->showPopup();
                handled = true;
            }
        } else if (renderTheme.popsMenuByArrowKeys()) {
            if (keyCode == ' ') {
                focus();

                if (!renderer() || !renderer()->isMenuList())
                    return;

                saveLastSelection();
                if (RenderMenuList* menuList = toRenderMenuList(renderer()))
                    menuList->showPopup();
                handled = true;
            } else if (keyCode == '\r') {
                if (form())
                    form()->submitImplicitly(event, false);
                dispatchChangeEventForMenuList();
                handled = true;
            }
        }

        if (handled)
            event->setDefaultHandled();
    }

    if (event->type() == EventTypeNames::mousedown && event->isMouseEvent() && toMouseEvent(event)->button() == LeftButton) {
        focus();
        if (renderer() && renderer()->isMenuList()) {
            if (RenderMenuList* menuList = toRenderMenuList(renderer())) {
                if (menuList->popupIsVisible())
                    menuList->hidePopup();
                else {
                    saveLastSelection();
                    menuList->showPopup();
                }
            }
        }
        event->setDefaultHandled();
    }

    if (event->type() == EventTypeNames::blur) {
        if (RenderMenuList* menuList = toRenderMenuList(renderer())) {
            if (menuList->popupIsVisible())
                menuList->hidePopup();
        }
    }
}

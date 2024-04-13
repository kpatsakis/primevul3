void HTMLSelectElement::listBoxDefaultEventHandler(Event* event)
{
    const Vector<HTMLElement*>& listItems = this->listItems();
    bool dragSelection = false;
    if (event->type() == EventTypeNames::mousedown && event->isMouseEvent() && toMouseEvent(event)->button() == LeftButton) {
        focus();
        if (!renderer())
            return;

        MouseEvent* mouseEvent = toMouseEvent(event);
        IntPoint localOffset = roundedIntPoint(renderer()->absoluteToLocal(mouseEvent->absoluteLocation(), UseTransforms));
        int listIndex = toRenderListBox(renderer())->listIndexAtOffset(toIntSize(localOffset));
        if (listIndex >= 0) {
            if (!isDisabledFormControl()) {
#if OS(MACOSX)
                updateSelectedState(listIndex, mouseEvent->metaKey(), mouseEvent->shiftKey());
#else
                updateSelectedState(listIndex, mouseEvent->ctrlKey(), mouseEvent->shiftKey());
#endif
            }
            if (Frame* frame = document().frame())
                frame->eventHandler().setMouseDownMayStartAutoscroll();

            event->setDefaultHandled();
        }
    } else if (event->type() == EventTypeNames::mousemove && event->isMouseEvent() && !toRenderBox(renderer())->canBeScrolledAndHasScrollableArea()) {
        MouseEvent* mouseEvent = toMouseEvent(event);
        if (mouseEvent->button() != LeftButton || !mouseEvent->buttonDown())
            return;

        IntPoint localOffset = roundedIntPoint(renderer()->absoluteToLocal(mouseEvent->absoluteLocation(), UseTransforms));
        int listIndex = toRenderListBox(renderer())->listIndexAtOffset(toIntSize(localOffset));
        if (listIndex >= 0) {
            if (!isDisabledFormControl()) {
                if (m_multiple) {
                    if (m_activeSelectionAnchorIndex < 0)
                        return;

                    setActiveSelectionEndIndex(listIndex);
                    updateListBoxSelection(false);
                } else {
                    setActiveSelectionAnchorIndex(listIndex);
                    setActiveSelectionEndIndex(listIndex);
                    updateListBoxSelection(true);
                }
            }
            dragSelection = true;
        }
    } else if (event->type() == EventTypeNames::mouseup && event->isMouseEvent() && toMouseEvent(event)->button() == LeftButton && renderer() && !toRenderBox(renderer())->autoscrollInProgress()) {
        if (m_lastOnChangeSelection.isEmpty())
            return;
        if (!dragSelection) {
            listBoxOnChange();
        }
    } else if (event->type() == EventTypeNames::keydown) {
        if (!event->isKeyboardEvent())
            return;
        const String& keyIdentifier = toKeyboardEvent(event)->keyIdentifier();

        bool handled = false;
        int endIndex = 0;
        if (m_activeSelectionEndIndex < 0) {
            if (keyIdentifier == "Down" || keyIdentifier == "PageDown") {
                int startIndex = lastSelectedListIndex();
                handled = true;
                if (keyIdentifier == "Down")
                    endIndex = nextSelectableListIndex(startIndex);
                else
                    endIndex = nextSelectableListIndexPageAway(startIndex, SkipForwards);
            } else if (keyIdentifier == "Up" || keyIdentifier == "PageUp") {
                int startIndex = optionToListIndex(selectedIndex());
                handled = true;
                if (keyIdentifier == "Up")
                    endIndex = previousSelectableListIndex(startIndex);
                else
                    endIndex = nextSelectableListIndexPageAway(startIndex, SkipBackwards);
            }
        } else {
            if (keyIdentifier == "Down") {
                endIndex = nextSelectableListIndex(m_activeSelectionEndIndex);
                handled = true;
            } else if (keyIdentifier == "Up") {
                endIndex = previousSelectableListIndex(m_activeSelectionEndIndex);
                handled = true;
            } else if (keyIdentifier == "PageDown") {
                endIndex = nextSelectableListIndexPageAway(m_activeSelectionEndIndex, SkipForwards);
                handled = true;
            } else if (keyIdentifier == "PageUp") {
                endIndex = nextSelectableListIndexPageAway(m_activeSelectionEndIndex, SkipBackwards);
                handled = true;
            }
        }
        if (keyIdentifier == "Home") {
            endIndex = firstSelectableListIndex();
            handled = true;
        } else if (keyIdentifier == "End") {
            endIndex = lastSelectableListIndex();
            handled = true;
        }

        if (isSpatialNavigationEnabled(document().frame()))
            if (keyIdentifier == "Left" || keyIdentifier == "Right" || ((keyIdentifier == "Down" || keyIdentifier == "Up") && endIndex == m_activeSelectionEndIndex))
                return;

        if (endIndex >= 0 && handled) {
            saveLastSelection();

            ASSERT_UNUSED(listItems, !listItems.size() || static_cast<size_t>(endIndex) < listItems.size());
            setActiveSelectionEndIndex(endIndex);

            bool selectNewItem = !m_multiple || toKeyboardEvent(event)->shiftKey() || !isSpatialNavigationEnabled(document().frame());
            if (selectNewItem)
                m_activeSelectionState = true;
            bool deselectOthers = !m_multiple || (!toKeyboardEvent(event)->shiftKey() && selectNewItem);
            if (m_activeSelectionAnchorIndex < 0 || deselectOthers) {
                if (deselectOthers)
                    deselectItemsWithoutValidation();
                setActiveSelectionAnchorIndex(m_activeSelectionEndIndex);
            }

            toRenderListBox(renderer())->scrollToRevealElementAtListIndex(endIndex);
            if (selectNewItem) {
                updateListBoxSelection(deselectOthers);
                listBoxOnChange();
            } else
                scrollToSelection();

            event->setDefaultHandled();
        }
    } else if (event->type() == EventTypeNames::keypress) {
        if (!event->isKeyboardEvent())
            return;
        int keyCode = toKeyboardEvent(event)->keyCode();

        if (keyCode == '\r') {
            if (form())
                form()->submitImplicitly(event, false);
            event->setDefaultHandled();
        } else if (m_multiple && keyCode == ' ' && isSpatialNavigationEnabled(document().frame())) {
            m_activeSelectionState = !m_activeSelectionState;
            updateSelectedState(listToOptionIndex(m_activeSelectionEndIndex), true /*multi*/, false /*shift*/);
            listBoxOnChange();
            event->setDefaultHandled();
        }
    }
}

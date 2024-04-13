void HTMLSelectElement::restoreFormControlState(const FormControlState& state)
{
    recalcListItems();

    const Vector<HTMLElement*>& items = listItems();
    size_t itemsSize = items.size();
    if (!itemsSize)
        return;

    for (size_t i = 0; i < itemsSize; ++i) {
        if (!items[i]->hasLocalName(optionTag))
            continue;
        toHTMLOptionElement(items[i])->setSelectedState(false);
    }

    if (!multiple()) {
        size_t foundIndex = searchOptionsForValue(state[0], 0, itemsSize);
        if (foundIndex != kNotFound)
            toHTMLOptionElement(items[foundIndex])->setSelectedState(true);
    } else {
        size_t startIndex = 0;
        for (size_t i = 0; i < state.valueSize(); ++i) {
            const String& value = state[i];
            size_t foundIndex = searchOptionsForValue(value, startIndex, itemsSize);
            if (foundIndex == kNotFound)
                foundIndex = searchOptionsForValue(value, 0, startIndex);
            if (foundIndex == kNotFound)
                continue;
            toHTMLOptionElement(items[foundIndex])->setSelectedState(true);
            startIndex = foundIndex + 1;
        }
    }

    setOptionsChangedOnRenderer();
    setNeedsValidityCheck();
}

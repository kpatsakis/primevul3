size_t HTMLSelectElement::searchOptionsForValue(const String& value, size_t listIndexStart, size_t listIndexEnd) const
{
    const Vector<HTMLElement*>& items = listItems();
    size_t loopEndIndex = std::min(items.size(), listIndexEnd);
    for (size_t i = listIndexStart; i < loopEndIndex; ++i) {
        if (!items[i]->hasLocalName(optionTag))
            continue;
        if (toHTMLOptionElement(items[i])->value() == value)
            return i;
    }
    return kNotFound;
}

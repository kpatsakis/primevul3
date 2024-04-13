String HTMLSelectElement::optionAtIndex(int index) const
{
    const Vector<HTMLElement*>& items = listItems();

    HTMLElement* element = items[index];
    if (!element->hasTagName(optionTag) || toHTMLOptionElement(element)->isDisabledFormControl())
        return String();
    return toHTMLOptionElement(element)->textIndentedToRespectGroupLabel();
}

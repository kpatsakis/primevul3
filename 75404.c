void RenderMenuList::setTextFromOption(int optionIndex)
{
    HTMLSelectElement* select = toHTMLSelectElement(node());
    const Vector<HTMLElement*>& listItems = select->listItems();
    int size = listItems.size();

    int i = select->optionToListIndex(optionIndex);
    String text = "";
    if (i >= 0 && i < size) {
        Element* element = listItems[i];
        if (element->hasTagName(optionTag)) {
            text = toHTMLOptionElement(element)->textIndentedToRespectGroupLabel();
            m_optionStyle = element->renderStyle();
        }
    }

    setText(text.stripWhiteSpace());
    didUpdateActiveOption(optionIndex);
}

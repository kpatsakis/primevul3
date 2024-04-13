void HTMLSelectElement::setMultiple(bool multiple)
{
    bool oldMultiple = this->multiple();
    int oldSelectedIndex = selectedIndex();
    setAttribute(multipleAttr, multiple ? emptyAtom : nullAtom);

    if (oldMultiple != this->multiple())
        setSelectedIndex(oldSelectedIndex);
}

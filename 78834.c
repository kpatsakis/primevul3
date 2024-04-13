DOMTokenList* Element::classList()
{
    ElementRareData* data = ensureElementRareData();
    if (!data->classList())
        data->setClassList(ClassList::create(this));
    return data->classList();
}

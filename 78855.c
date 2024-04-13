DOMStringMap* Element::dataset()
{
    ElementRareData* data = ensureElementRareData();
    if (!data->dataset())
        data->setDataset(DatasetDOMStringMap::create(this));
    return data->dataset();
}

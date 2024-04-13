short Element::tabIndex() const
{
    return hasRareData() ? elementRareData()->tabIndex() : 0;
}

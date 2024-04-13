bool Element::supportsFocus() const
{
    return hasRareData() && elementRareData()->tabIndexSetExplicitly();
}

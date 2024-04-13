bool HTMLInputElement::sizeShouldIncludeDecoration(int& preferredSize) const
{
    return m_inputTypeView->sizeShouldIncludeDecoration(defaultSize, preferredSize);
}

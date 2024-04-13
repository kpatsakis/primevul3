void HTMLInputElement::subtreeHasChanged()
{
    m_inputTypeView->subtreeHasChanged();
    calculateAndAdjustDirectionality();
}

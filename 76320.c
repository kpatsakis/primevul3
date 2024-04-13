void InlineTextBox::attachLine()
{
    if (!m_extracted)
        return;
    
    toRenderText(renderer())->attachTextBox(this);
}

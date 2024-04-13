void InlineTextBox::extractLine()
{
    if (m_extracted)
        return;

    toRenderText(renderer())->extractTextBox(this);
}

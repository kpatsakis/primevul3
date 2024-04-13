int InlineTextBox::baselinePosition() const
{
    if (!isText() || !parent())
        return 0;
    return parent()->baselinePosition();
}

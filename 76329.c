bool InlineTextBox::isLineBreak() const
{
    return renderer()->isBR() || (renderer()->style()->preserveNewline() && len() == 1 && (*textRenderer()->text())[start()] == '\n');
}

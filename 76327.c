 void InlineTextBox::deleteLine(RenderArena* arena)
{
    toRenderText(renderer())->removeTextBox(this);
    destroy(arena);
}

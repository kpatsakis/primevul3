void InlineTextBox::paintCustomHighlight(int tx, int ty, const AtomicString& type)
{
    Frame* frame = renderer()->frame();
    if (!frame)
        return;
    Page* page = frame->page();
    if (!page)
        return;

    RootInlineBox* r = root();
    FloatRect rootRect(tx + r->x(), ty + selectionTop(), r->logicalWidth(), selectionHeight());
    FloatRect textRect(tx + x(), rootRect.y(), logicalWidth(), rootRect.height());

    page->chrome()->client()->paintCustomHighlight(renderer()->node(), type, textRect, rootRect, true, false);
}

static inline void dirtyLineBoxesForRenderer(RenderObject* o, bool fullLayout)
{
    if (o->isText()) {
        RenderText* renderText = toRenderText(o);
        renderText->dirtyLineBoxes(fullLayout);
    } else
        toRenderInline(o)->dirtyLineBoxes(fullLayout);
}

PassRefPtr<RenderStyle> Element::styleForRenderer()
{
    if (hasCustomStyleCallbacks()) {
        if (RefPtr<RenderStyle> style = customStyleForRenderer())
            return style.release();
    }

    return originalStyleForRenderer();
}

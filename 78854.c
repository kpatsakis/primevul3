PassRefPtr<RenderStyle> Element::customStyleForRenderer()
{
    ASSERT(hasCustomStyleCallbacks());
    return 0;
}

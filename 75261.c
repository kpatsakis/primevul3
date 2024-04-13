void TextAutosizer::setMultiplier(RenderObject* renderer, float multiplier)
{
    RefPtr<RenderStyle> newStyle = RenderStyle::clone(renderer->style());
    newStyle->setTextAutosizingMultiplier(multiplier);
    renderer->setStyle(newStyle.release());
}

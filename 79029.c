void Element::willRecalcStyle(StyleChange)
{
    ASSERT(hasCustomStyleCallbacks());
}

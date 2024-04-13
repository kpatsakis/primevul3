bool Element::fastAttributeLookupAllowed(const QualifiedName& name) const
{
    if (name == HTMLNames::styleAttr)
        return false;

    if (isSVGElement())
        return !static_cast<const SVGElement*>(this)->isAnimatableAttribute(name);

    return true;
}

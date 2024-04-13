inline void Element::synchronizeAttribute(const QualifiedName& name) const
{
    if (!elementData())
        return;
    if (UNLIKELY(name == styleAttr && elementData()->m_styleAttributeIsDirty)) {
        ASSERT(isStyledElement());
        static_cast<const StyledElement*>(this)->synchronizeStyleAttributeInternal();
        return;
    }
    if (UNLIKELY(elementData()->m_animatedSVGAttributesAreDirty)) {
        ASSERT(isSVGElement());
        toSVGElement(this)->synchronizeAnimatedSVGAttribute(name);
    }
}

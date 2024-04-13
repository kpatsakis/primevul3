void Element::didAddAttribute(const QualifiedName& name, const AtomicString& value)
{
    attributeChanged(name, value);
    InspectorInstrumentation::didModifyDOMAttr(document(), this, name.localName(), value);
    dispatchSubtreeModifiedEvent();
}

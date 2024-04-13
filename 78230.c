void HTMLFormElement::anonymousNamedGetter(const AtomicString& name, bool& returnValue0Enabled, RefPtr<NodeList>& returnValue0, bool& returnValue1Enabled, RefPtr<Node>& returnValue1)
{
    {
        Vector<RefPtr<Node> > elements;
        getNamedElements(name, elements);
        if (elements.isEmpty())
            return;
    }

    Vector<RefPtr<Node> > elements;
    getNamedElements(name, elements);
    ASSERT(!elements.isEmpty());

    if (elements.size() == 1) {
        returnValue1Enabled = true;
        returnValue1 = elements.at(0);
        return;
    }

    returnValue0Enabled = true;
    returnValue0 = NamedNodesCollection::create(elements);
}

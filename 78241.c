void HTMLFormElement::getNamedElements(const AtomicString& name, Vector<RefPtr<Node> >& namedItems)
{
    elements()->namedItems(name, namedItems);

    Node* elementFromPast = elementFromPastNamesMap(name);
    if (namedItems.size() && namedItems.first() != elementFromPast)
        addToPastNamesMap(namedItems.first().get(), name);
    else if (elementFromPast && namedItems.isEmpty())
        namedItems.append(elementFromPast);
}

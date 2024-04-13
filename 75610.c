static void dispatchChildInsertionEvents(Node* child)
{
    if (child->isInShadowTree())
        return;

    ASSERT(!eventDispatchForbidden());

    RefPtr<Node> c = child;
    RefPtr<Document> document = child->document();

    if (c->parentNode() && document->hasListenerType(Document::DOMNODEINSERTED_LISTENER))
        c->dispatchScopedEvent(MutationEvent::create(eventNames().DOMNodeInsertedEvent, true, c->parentNode()));

    if (c->inDocument() && document->hasListenerType(Document::DOMNODEINSERTEDINTODOCUMENT_LISTENER)) {
        for (; c; c = c->traverseNextNode(child))
            c->dispatchScopedEvent(MutationEvent::create(eventNames().DOMNodeInsertedIntoDocumentEvent, false));
    }
}

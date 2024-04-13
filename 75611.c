static void dispatchChildRemovalEvents(Node* child)
{
    if (child->isInShadowTree())
        return;

    ASSERT(!eventDispatchForbidden());

    InspectorInstrumentation::willRemoveDOMNode(child->document(), child);

    RefPtr<Node> c = child;
    RefPtr<Document> document = child->document();

    if (c->parentNode() && document->hasListenerType(Document::DOMNODEREMOVED_LISTENER))
        c->dispatchScopedEvent(MutationEvent::create(eventNames().DOMNodeRemovedEvent, true, c->parentNode()));

    if (c->inDocument() && document->hasListenerType(Document::DOMNODEREMOVEDFROMDOCUMENT_LISTENER)) {
        for (; c; c = c->traverseNextNode(child))
            c->dispatchScopedEvent(MutationEvent::create(eventNames().DOMNodeRemovedFromDocumentEvent, false));
    }
}

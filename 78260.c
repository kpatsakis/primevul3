void HTMLFormElement::requestAutocompleteTimerFired(Timer<HTMLFormElement>*)
{
    Vector<RefPtr<Event> > pendingEvents;
    m_pendingAutocompleteEvents.swap(pendingEvents);
    for (size_t i = 0; i < pendingEvents.size(); ++i)
        dispatchEvent(pendingEvents[i].release());
}

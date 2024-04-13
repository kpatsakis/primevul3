void HTMLFormElement::setDemoted(bool demoted)
{
    if (demoted)
        UseCounter::count(&document(), UseCounter::DemotedFormElement);
    m_wasDemoted = demoted;
}

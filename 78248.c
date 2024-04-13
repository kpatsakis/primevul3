String HTMLFormElement::method() const
{
    return FormSubmission::Attributes::methodString(m_attributes.method());
}

HTMLFormElement::~HTMLFormElement()
{
    document().formController()->willDeleteForm(this);

    for (unsigned i = 0; i < m_associatedElements.size(); ++i)
        m_associatedElements[i]->formWillBeDestroyed();
    for (unsigned i = 0; i < m_imageElements.size(); ++i)
        m_imageElements[i]->m_form = 0;
}

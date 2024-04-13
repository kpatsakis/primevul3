 void HTMLFormControlElement::setAutofilled(bool autofilled)
 {
     if (autofilled == m_isAutofilled)
        return;

    m_isAutofilled = autofilled;
    setNeedsStyleRecalc();
}

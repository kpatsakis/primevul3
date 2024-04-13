void HTMLFormControlElement::requiredAttributeChanged()
{
    setNeedsValidityCheck();
     setNeedsStyleRecalc();
 }

  void ExpectFilledTestForm() {
    ExpectFieldValue(L"firstname", "Milton");
    ExpectFieldValue(L"lastname", "Waddams");
    ExpectFieldValue(L"address1", "4120 Freidrich Lane");
    ExpectFieldValue(L"address2", "Basement");
    ExpectFieldValue(L"city", "Austin");
    ExpectFieldValue(L"state", "TX");
    ExpectFieldValue(L"zip", "78744");
    ExpectFieldValue(L"country", "US");
     ExpectFieldValue(L"phone", "5125551234");
   }

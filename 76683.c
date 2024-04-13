  void CreateTestProfile() {
    autofill_test::DisableSystemServices(browser()->profile());

    AutofillProfile profile;
    autofill_test::SetProfileInfo(
        &profile, "Milton", "C.", "Waddams",
        "red.swingline@initech.com", "Initech", "4120 Freidrich Lane",
        "Basement", "Austin", "Texas", "78744", "United States", "5125551234");

    PersonalDataManager* personal_data_manager =
        PersonalDataManagerFactory::GetForProfile(browser()->profile());
    ASSERT_TRUE(personal_data_manager);

    WindowedPersonalDataManagerObserver observer;
    personal_data_manager->SetObserver(&observer);

    personal_data_manager->AddProfile(profile);

    observer.Wait();
    personal_data_manager->RemoveObserver(&observer);
  }

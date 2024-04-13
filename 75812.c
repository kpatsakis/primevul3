BrowserSignin* GetBrowserSignin(Profile* profile) {
  if (test_signin)
    return test_signin;
  else
    return profile->GetBrowserSignin();
}

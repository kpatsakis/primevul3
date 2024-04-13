bool GetTestUrlForAndroid(std::string& path_or_url, GURL* url) {
  if (path_or_url.find(kAndroidLayoutTestPath) == std::string::npos)
    return false;

  std::string test_location(kAndroidLayoutTestBase);
  test_location.append(path_or_url.substr(strlen(kAndroidLayoutTestPath)));

  *url = GURL(test_location);
  return true;
 }

void test_file_descriptor()
{
  YR_COMPILER* compiler = NULL;
  YR_RULES* rules = NULL;
  
#if defined(_WIN32) || defined(__CYGWIN__)
  HANDLE fd = CreateFile("tests/data/true.yar", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
  if (fd == INVALID_HANDLE_VALUE)
  {
    fputs("CreateFile failed", stderr);
    exit(1);
  }
#else
  int fd = open("tests/data/true.yar", O_RDONLY);
  if (fd < 0)
  {
    perror("open");
    exit(EXIT_FAILURE);
  }
#endif
  if (yr_compiler_create(&compiler) != ERROR_SUCCESS)
  {
    perror("yr_compiler_create");
    exit(EXIT_FAILURE);
  }

  if (yr_compiler_add_fd(compiler, fd, NULL, NULL) != 0) {
    perror("yr_compiler_add_fd");
    exit(EXIT_FAILURE);
  }
  
#if defined(_WIN32) || defined(__CYGWIN__)
  CloseHandle(fd);
#else
  close(fd);
#endif

  if (yr_compiler_get_rules(compiler, &rules) != ERROR_SUCCESS) {
    perror("yr_compiler_add_fd");
    exit(EXIT_FAILURE);
  }

  if (compiler)
  {
    yr_compiler_destroy(compiler);
  }
  if (rules)
  {
    yr_rules_destroy(rules);
  }
  
  return;
}

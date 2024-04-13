bool GetNextTest(const CommandLine::StringVector& args,
                 size_t* position,
                 std::string* test) {
  if (*position >= args.size())
    return false;
  if (args[*position] == FILE_PATH_LITERAL("-"))
    return !!std::getline(std::cin, *test, '\n');
#if defined(OS_WIN)
  *test = base::WideToUTF8(args[(*position)++]);
#else
  *test = args[(*position)++];
#endif
  return true;
}

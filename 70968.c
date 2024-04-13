dump_info (int fd, const char *output, bool exit_on_error)
{
  size_t len = strlen (output);
  if (write_to_fd (fd, output, len))
    {
      if (exit_on_error)
        die_with_error ("Write to info_fd");
    }
}

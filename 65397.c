static int fetch_message(char *line, void *file)
{
  FILE *f = (FILE *) file;

  fputs(line, f);
  if (fputc('\n', f) == EOF)
    return -1;

  return 0;
}

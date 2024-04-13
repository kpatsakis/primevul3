static void StripStyleTokens(char *message)
{
  register char
    *p,
    *q;

  size_t
    length;

  assert(message != (char *) NULL);
  if (*message == '\0')
    return;
  length=strlen(message);
  p=message;
  while (isspace((int) ((unsigned char) *p)) != 0)
    p++;
  q=message+length-1;
  while ((isspace((int) ((unsigned char) *q)) != 0) && (q > p))
    q--;
  (void) CopyMagickMemory(message,p,(size_t) (q-p+1));
  message[q-p+1]='\0';
  StripString(message);
}

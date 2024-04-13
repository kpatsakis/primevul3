uint8_t escaped_char_value(
    char* text)
{
  char hex[3];
  int result;

  assert(text[0] == '\\');

  switch(text[1])
  {
  case 'x':
    hex[0] = text[2];
    hex[1] = text[3];
    hex[2] = '\0';
    sscanf(hex, "%x", &result);
    break;

  case 'n':
    result = '\n';
    break;

  case 't':
    result = '\t';
    break;

  case 'r':
    result = '\r';
    break;

  case 'f':
    result = '\f';
    break;

  case 'a':
    result = '\a';
    break;

  default:
    result = text[1];
  }

  return result;
}

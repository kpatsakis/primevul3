cmsBool ismiddle(int c)
{
   return (!isseparator(c) && (c != '#') && (c !='\"') && (c != '\'') && (c > 32) && (c < 127));
}

static void freecookie(struct Cookie *co)
{
  free(co->expirestr);
  free(co->domain);
  free(co->path);
  free(co->spath);
  free(co->name);
  free(co->value);
  free(co->maxage);
  free(co->version);
  free(co);
}
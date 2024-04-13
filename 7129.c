static int cookie_sort_ct(const void *p1, const void *p2)
{
  struct Cookie *c1 = *(struct Cookie **)p1;
  struct Cookie *c2 = *(struct Cookie **)p2;

  return (c2->creationtime > c1->creationtime) ? 1 : -1;
}
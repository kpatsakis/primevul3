__realpath (const char *name, char *resolved)
{
  #ifdef GCC_BOGUS_WRETURN_LOCAL_ADDR
   #warning "GCC might issue a bogus -Wreturn-local-addr warning here."
   #warning "See <https://gcc.gnu.org/bugzilla/show_bug.cgi?id=93644>."
  #endif
  struct scratch_buffer rname_buffer;
  return realpath_stk (name, resolved, &rname_buffer);
}
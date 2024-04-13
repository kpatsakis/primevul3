static void sasl_timeout_callback(struct Event* ev)
{
  struct Client *cptr;

  assert(0 != ev_timer(ev));
  assert(0 != t_data(ev_timer(ev)));

  if (ev_type(ev) == ET_EXPIRE) {
    cptr = (struct Client*) t_data(ev_timer(ev));

   abort_sasl(cptr, 1);
  }
}

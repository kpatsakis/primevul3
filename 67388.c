stack_extend_alloc(mrb_state *mrb, int room)
{
  mrb_value *oldbase = mrb->c->stbase;
  mrb_value *newstack;
  size_t oldsize = mrb->c->stend - mrb->c->stbase;
  size_t size = oldsize;
  size_t off = mrb->c->stack - mrb->c->stbase;

  if (off > size) size = off;
#ifdef MRB_STACK_EXTEND_DOUBLING
  if (room <= size)
    size *= 2;
  else
    size += room;
#else
  /* Use linear stack growth.
     It is slightly slower than doubling the stack space,
     but it saves memory on small devices. */
  if (room <= MRB_STACK_GROWTH)
    size += MRB_STACK_GROWTH;
  else
    size += room;
#endif

  newstack = (mrb_value *)mrb_realloc(mrb, mrb->c->stbase, sizeof(mrb_value) * size);
  if (newstack == NULL) {
    mrb_exc_raise(mrb, mrb_obj_value(mrb->stack_err));
  }
  stack_clear(&(newstack[oldsize]), size - oldsize);
  envadjust(mrb, oldbase, newstack, size);
  mrb->c->stbase = newstack;
  mrb->c->stack = mrb->c->stbase + off;
  mrb->c->stend = mrb->c->stbase + size;

  /* Raise an exception if the new stack size will be too large,
     to prevent infinite recursion. However, do this only after resizing the stack, so mrb_raise has stack space to work with. */
  if (size > MRB_STACK_MAX) {
    mrb_exc_raise(mrb, mrb_obj_value(mrb->stack_err));
  }
}

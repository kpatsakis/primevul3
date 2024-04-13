mrb_io_s_select(mrb_state *mrb, mrb_value klass)
{
  mrb_value *argv;
  mrb_int argc;
  mrb_value read, read_io, write, except, timeout, list;
  struct timeval *tp, timerec;
  fd_set pset, rset, wset, eset;
  fd_set *rp, *wp, *ep;
  struct mrb_io *fptr;
  int pending = 0;
  mrb_value result;
  int max = 0;
  int interrupt_flag = 0;
  int i, n;

  mrb_get_args(mrb, "*", &argv, &argc);

  if (argc < 1 || argc > 4) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "wrong number of arguments (%S for 1..4)", mrb_fixnum_value(argc));
  }

  timeout = mrb_nil_value();
  except = mrb_nil_value();
  write = mrb_nil_value();
  if (argc > 3)
    timeout = argv[3];
  if (argc > 2)
    except = argv[2];
  if (argc > 1)
    write = argv[1];
  read = argv[0];

  if (mrb_nil_p(timeout)) {
    tp = NULL;
  } else {
    timerec = time2timeval(mrb, timeout);
    tp = &timerec;
  }

  FD_ZERO(&pset);
  if (!mrb_nil_p(read)) {
    mrb_check_type(mrb, read, MRB_TT_ARRAY);
    rp = &rset;
    FD_ZERO(rp);
    for (i = 0; i < RARRAY_LEN(read); i++) {
      read_io = RARRAY_PTR(read)[i];
      fptr = (struct mrb_io *)mrb_get_datatype(mrb, read_io, &mrb_io_type);
      FD_SET(fptr->fd, rp);
      if (mrb_io_read_data_pending(mrb, read_io)) {
        pending++;
        FD_SET(fptr->fd, &pset);
      }
      if (max < fptr->fd)
        max = fptr->fd;
    }
    if (pending) {
      timerec.tv_sec = timerec.tv_usec = 0;
      tp = &timerec;
    }
  } else {
    rp = NULL;
  }

  if (!mrb_nil_p(write)) {
    mrb_check_type(mrb, write, MRB_TT_ARRAY);
    wp = &wset;
    FD_ZERO(wp);
    for (i = 0; i < RARRAY_LEN(write); i++) {
      fptr = (struct mrb_io *)mrb_get_datatype(mrb, RARRAY_PTR(write)[i], &mrb_io_type);
      FD_SET(fptr->fd, wp);
      if (max < fptr->fd)
        max = fptr->fd;
      if (fptr->fd2 >= 0) {
        FD_SET(fptr->fd2, wp);
        if (max < fptr->fd2)
          max = fptr->fd2;
      }
    }
  } else {
    wp = NULL;
  }

  if (!mrb_nil_p(except)) {
    mrb_check_type(mrb, except, MRB_TT_ARRAY);
    ep = &eset;
    FD_ZERO(ep);
    for (i = 0; i < RARRAY_LEN(except); i++) {
      fptr = (struct mrb_io *)mrb_get_datatype(mrb, RARRAY_PTR(except)[i], &mrb_io_type);
      FD_SET(fptr->fd, ep);
      if (max < fptr->fd)
        max = fptr->fd;
      if (fptr->fd2 >= 0) {
        FD_SET(fptr->fd2, ep);
        if (max < fptr->fd2)
          max = fptr->fd2;
      }
    }
  } else {
    ep = NULL;
  }

  max++;

retry:
  n = select(max, rp, wp, ep, tp);
  if (n < 0) {
    if (errno != EINTR)
      mrb_sys_fail(mrb, "select failed");
    if (tp == NULL)
      goto retry;
    interrupt_flag = 1;
  }

  if (!pending && n == 0)
    return mrb_nil_value();

  result = mrb_ary_new_capa(mrb, 3);
  mrb_ary_push(mrb, result, rp? mrb_ary_new(mrb) : mrb_ary_new_capa(mrb, 0));
  mrb_ary_push(mrb, result, wp? mrb_ary_new(mrb) : mrb_ary_new_capa(mrb, 0));
  mrb_ary_push(mrb, result, ep? mrb_ary_new(mrb) : mrb_ary_new_capa(mrb, 0));

  if (interrupt_flag == 0) {
    if (rp) {
      list = RARRAY_PTR(result)[0];
      for (i = 0; i < RARRAY_LEN(read); i++) {
        fptr = (struct mrb_io *)mrb_get_datatype(mrb, RARRAY_PTR(read)[i], &mrb_io_type);
        if (FD_ISSET(fptr->fd, rp) ||
            FD_ISSET(fptr->fd, &pset)) {
          mrb_ary_push(mrb, list, RARRAY_PTR(read)[i]);
        }
      }
    }

    if (wp) {
      list = RARRAY_PTR(result)[1];
      for (i = 0; i < RARRAY_LEN(write); i++) {
        fptr = (struct mrb_io *)mrb_get_datatype(mrb, RARRAY_PTR(write)[i], &mrb_io_type);
        if (FD_ISSET(fptr->fd, wp)) {
          mrb_ary_push(mrb, list, RARRAY_PTR(write)[i]);
        } else if (fptr->fd2 >= 0 && FD_ISSET(fptr->fd2, wp)) {
          mrb_ary_push(mrb, list, RARRAY_PTR(write)[i]);
        }
      }
    }

    if (ep) {
      list = RARRAY_PTR(result)[2];
      for (i = 0; i < RARRAY_LEN(except); i++) {
        fptr = (struct mrb_io *)mrb_get_datatype(mrb, RARRAY_PTR(except)[i], &mrb_io_type);
        if (FD_ISSET(fptr->fd, ep)) {
          mrb_ary_push(mrb, list, RARRAY_PTR(except)[i]);
        } else if (fptr->fd2 >= 0 && FD_ISSET(fptr->fd2, ep)) {
          mrb_ary_push(mrb, list, RARRAY_PTR(except)[i]);
        }
      }
    }
  }

  return result;
}

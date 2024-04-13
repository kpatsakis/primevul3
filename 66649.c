mrb_get_args(mrb_state *mrb, const char *format, ...)
{
  const char *fmt = format;
  char c;
  mrb_int i = 0;
  va_list ap;
  mrb_int argc = mrb_get_argc(mrb);
  mrb_int arg_i = 0;
  mrb_value *array_argv = mrb_get_argv(mrb);
  mrb_bool opt = FALSE;
  mrb_bool opt_skip = TRUE;
  mrb_bool given = TRUE;

  va_start(ap, format);

#define ARGV \
  (array_argv ? array_argv : (mrb->c->stack + 1))

  while ((c = *fmt++)) {
    switch (c) {
    case '|':
      opt = TRUE;
      break;
    case '*':
      opt_skip = FALSE;
      goto check_exit;
    case '!':
      break;
    case '&': case '?':
      if (opt) opt_skip = FALSE;
      break;
    default:
      break;
    }
  }

 check_exit:
  opt = FALSE;
  i = 0;
  while ((c = *format++)) {
    switch (c) {
    case '|': case '*': case '&': case '?':
      break;
    default:
      if (argc <= i) {
        if (opt) {
          given = FALSE;
        }
        else {
          mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments");
        }
      }
      break;
    }

    switch (c) {
    case 'o':
      {
        mrb_value *p;

        p = va_arg(ap, mrb_value*);
        if (i < argc) {
          *p = ARGV[arg_i++];
          i++;
        }
      }
      break;
    case 'C':
      {
        mrb_value *p;

        p = va_arg(ap, mrb_value*);
        if (i < argc) {
          mrb_value ss;

          ss = ARGV[arg_i++];
          if (!class_ptr_p(ss)) {
            mrb_raisef(mrb, E_TYPE_ERROR, "%S is not class/module", ss);
          }
          *p = ss;
          i++;
        }
      }
      break;
    case 'S':
      {
        mrb_value *p;

        p = va_arg(ap, mrb_value*);
        if (*format == '!') {
          format++;
          if (i < argc && mrb_nil_p(ARGV[arg_i])) {
            *p = ARGV[arg_i++];
            i++;
            break;
          }
        }
        if (i < argc) {
          *p = to_str(mrb, ARGV[arg_i++]);
          i++;
        }
      }
      break;
    case 'A':
      {
        mrb_value *p;

        p = va_arg(ap, mrb_value*);
        if (*format == '!') {
          format++;
          if (i < argc && mrb_nil_p(ARGV[arg_i])) {
            *p = ARGV[arg_i++];
            i++;
            break;
          }
        }
        if (i < argc) {
          *p = to_ary(mrb, ARGV[arg_i++]);
          i++;
        }
      }
      break;
    case 'H':
      {
        mrb_value *p;

        p = va_arg(ap, mrb_value*);
        if (*format == '!') {
          format++;
          if (i < argc && mrb_nil_p(ARGV[arg_i])) {
            *p = ARGV[arg_i++];
            i++;
            break;
          }
        }
        if (i < argc) {
          *p = to_hash(mrb, ARGV[arg_i++]);
          i++;
        }
      }
      break;
    case 's':
      {
        mrb_value ss;
        char **ps = 0;
        mrb_int *pl = 0;

        ps = va_arg(ap, char**);
        pl = va_arg(ap, mrb_int*);
        if (*format == '!') {
          format++;
          if (i < argc && mrb_nil_p(ARGV[arg_i])) {
            *ps = NULL;
            *pl = 0;
            i++; arg_i++;
            break;
          }
        }
        if (i < argc) {
          ss = to_str(mrb, ARGV[arg_i++]);
          *ps = RSTRING_PTR(ss);
          *pl = RSTRING_LEN(ss);
          i++;
        }
      }
      break;
    case 'z':
      {
        mrb_value ss;
        const char **ps;

        ps = va_arg(ap, const char**);
        if (*format == '!') {
          format++;
          if (i < argc && mrb_nil_p(ARGV[arg_i])) {
            *ps = NULL;
            i++; arg_i++;
            break;
          }
        }
        if (i < argc) {
          ss = to_str(mrb, ARGV[arg_i++]);
          *ps = mrb_string_value_cstr(mrb, &ss);
          i++;
        }
      }
      break;
    case 'a':
      {
        mrb_value aa;
        struct RArray *a;
        mrb_value **pb;
        mrb_int *pl;

        pb = va_arg(ap, mrb_value**);
        pl = va_arg(ap, mrb_int*);
        if (*format == '!') {
          format++;
          if (i < argc && mrb_nil_p(ARGV[arg_i])) {
            *pb = 0;
            *pl = 0;
            i++; arg_i++;
            break;
          }
        }
        if (i < argc) {
          aa = to_ary(mrb, ARGV[arg_i++]);
          a = mrb_ary_ptr(aa);
          *pb = ARY_PTR(a);
          *pl = ARY_LEN(a);
          i++;
        }
      }
      break;
    case 'I':
      {
        void* *p;
        mrb_value ss;

        p = va_arg(ap, void**);
        if (i < argc) {
          ss = ARGV[arg_i];
          if (mrb_type(ss) != MRB_TT_ISTRUCT)
          {
            mrb_raisef(mrb, E_TYPE_ERROR, "%S is not inline struct", ss);
          }
          *p = mrb_istruct_ptr(ss);
          arg_i++;
          i++;
        }
      }
      break;
#ifndef MRB_WITHOUT_FLOAT
    case 'f':
      {
        mrb_float *p;

        p = va_arg(ap, mrb_float*);
        if (i < argc) {
          *p = mrb_to_flo(mrb, ARGV[arg_i]);
          arg_i++;
          i++;
        }
      }
      break;
#endif
    case 'i':
      {
        mrb_int *p;

        p = va_arg(ap, mrb_int*);
        if (i < argc) {
          switch (mrb_type(ARGV[arg_i])) {
            case MRB_TT_FIXNUM:
              *p = mrb_fixnum(ARGV[arg_i]);
              break;
#ifndef MRB_WITHOUT_FLOAT
            case MRB_TT_FLOAT:
              {
                mrb_float f = mrb_float(ARGV[arg_i]);

                if (!FIXABLE_FLOAT(f)) {
                  mrb_raise(mrb, E_RANGE_ERROR, "float too big for int");
                }
                *p = (mrb_int)f;
              }
              break;
#endif
            case MRB_TT_STRING:
              mrb_raise(mrb, E_TYPE_ERROR, "no implicit conversion of String into Integer");
              break;
            default:
              *p = mrb_fixnum(mrb_Integer(mrb, ARGV[arg_i]));
              break;
          }
          arg_i++;
          i++;
        }
      }
      break;
    case 'b':
      {
        mrb_bool *boolp = va_arg(ap, mrb_bool*);

        if (i < argc) {
          mrb_value b = ARGV[arg_i++];
          *boolp = mrb_test(b);
          i++;
        }
      }
      break;
    case 'n':
      {
        mrb_sym *symp;

        symp = va_arg(ap, mrb_sym*);
        if (i < argc) {
          mrb_value ss;

          ss = ARGV[arg_i++];
          *symp = to_sym(mrb, ss);
          i++;
        }
      }
      break;
    case 'd':
      {
        void** datap;
        struct mrb_data_type const* type;

        datap = va_arg(ap, void**);
        type = va_arg(ap, struct mrb_data_type const*);
        if (*format == '!') {
          format++;
          if (i < argc && mrb_nil_p(ARGV[arg_i])) {
            *datap = 0;
            i++; arg_i++;
            break;
          }
        }
        if (i < argc) {
          *datap = mrb_data_get_ptr(mrb, ARGV[arg_i++], type);
          ++i;
        }
      }
      break;

    case '&':
      {
        mrb_value *p, *bp;

        p = va_arg(ap, mrb_value*);
        if (mrb->c->ci->argc < 0) {
          bp = mrb->c->stack + 2;
        }
        else {
          bp = mrb->c->stack + mrb->c->ci->argc + 1;
        }
        if (*format == '!') {
          format ++;
          if (mrb_nil_p(*bp)) {
            mrb_raise(mrb, E_ARGUMENT_ERROR, "no block given");
          }
        }
        *p = *bp;
      }
      break;
    case '|':
      if (opt_skip && i == argc) return argc;
      opt = TRUE;
      break;
    case '?':
      {
        mrb_bool *p;

        p = va_arg(ap, mrb_bool*);
        *p = given;
      }
      break;

    case '*':
      {
        mrb_value **var;
        mrb_int *pl;
        mrb_bool nocopy = array_argv ? TRUE : FALSE;

        if (*format == '!') {
          format++;
          nocopy = TRUE;
        }
        var = va_arg(ap, mrb_value**);
        pl = va_arg(ap, mrb_int*);
        if (argc > i) {
          *pl = argc-i;
          if (*pl > 0) {
            if (nocopy) {
              *var = ARGV+arg_i;
            }
            else {
              mrb_value args = mrb_ary_new_from_values(mrb, *pl, ARGV+arg_i);
              RARRAY(args)->c = NULL;
              *var = RARRAY_PTR(args);
            }
          }
          i = argc;
          arg_i += *pl;
        }
        else {
          *pl = 0;
          *var = NULL;
        }
      }
      break;
    default:
      mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid argument specifier %S", mrb_str_new(mrb, &c, 1));
      break;
    }
  }

#undef ARGV

  if (!c && argc > i) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments");
  }
  va_end(ap);
  return i;
}

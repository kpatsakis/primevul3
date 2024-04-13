static void _anal_calls(RCore *core, ut64 addr, ut64 addr_end) {
	RAnalOp op;
	int bufi;
	int depth = r_config_get_i (core->config, "anal.depth");
	const int addrbytes = core->io->addrbytes;
	const int bsz = 4096;
	ut8 *buf;
	ut8 *block;
	bufi = 0;
	if (addr_end - addr > UT32_MAX) {
		return;
	}
	buf = malloc (bsz);
	block = malloc (bsz);
	if (!buf || !block) {
		eprintf ("Error: cannot allocate buf or block\n");
		free (buf);
		free (block);
		return;
	}

	int minop = r_anal_archinfo (core->anal, R_ANAL_ARCHINFO_MIN_OP_SIZE);
	if (minop < 1) {
		minop = 1;
	}
	while (addr < addr_end) {
		if (r_cons_is_breaked ()) {
			break;
		}
		if (bufi > 4000) {
			bufi = 0;
		}
		if (!bufi) {
			r_io_read_at (core->io, addr, buf, bsz);
		}
		memset (block, -1, bsz);
		if (!memcmp (buf, block, bsz)) {
			addr += bsz;
			continue;
		}
		memset (block, 0, bsz);
		if (!memcmp (buf, block, bsz)) {
			addr += bsz;
			continue;
		}
		if (r_anal_op (core->anal, &op, addr, buf + bufi, bsz - bufi, 0) > 0) {
			if (op.size < 1) {
				op.size = minop;
			}
			if (op.type == R_ANAL_OP_TYPE_CALL) {
#if JAYRO_03
#error FUCK
				if (!anal_is_bad_call (core, from, to, addr, buf, bufi)) {
					fcn = r_anal_get_fcn_in (core->anal, op.jump, R_ANAL_FCN_TYPE_ROOT);
					if (!fcn) {
						r_core_anal_fcn (core, op.jump, addr,
						  R_ANAL_REF_TYPE_NULL, depth);
					}
				}
#else
				r_anal_xrefs_set (core->anal, R_ANAL_REF_TYPE_CALL, addr, op.jump);
				if (r_io_is_valid_offset (core->io, op.jump, 1)) {
					r_core_anal_fcn (core, op.jump, addr, R_ANAL_REF_TYPE_NULL, depth);
				}
#endif
			}

		} else {
			op.size = minop;
		}
		if ((int)op.size < 1) {
			op.size = minop;
		}
		addr += op.size;
		bufi += addrbytes * op.size;
		r_anal_op_fini (&op);
	}
	free (buf);
	free (block);
}

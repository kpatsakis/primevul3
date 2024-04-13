static int cx24116_set_frontend(struct dvb_frontend *fe)
{
	struct cx24116_state *state = fe->demodulator_priv;
	struct dtv_frontend_properties *c = &fe->dtv_property_cache;
	struct cx24116_cmd cmd;
	fe_status_t tunerstat;
	int i, status, ret, retune = 1;

	dprintk("%s()\n", __func__);

	switch (c->delivery_system) {
	case SYS_DVBS:
		dprintk("%s: DVB-S delivery system selected\n", __func__);

		/* Only QPSK is supported for DVB-S */
		if (c->modulation != QPSK) {
			dprintk("%s: unsupported modulation selected (%d)\n",
				__func__, c->modulation);
			return -EOPNOTSUPP;
		}

		/* Pilot doesn't exist in DVB-S, turn bit off */
		state->dnxt.pilot_val = CX24116_PILOT_OFF;

		/* DVB-S only supports 0.35 */
		if (c->rolloff != ROLLOFF_35) {
			dprintk("%s: unsupported rolloff selected (%d)\n",
				__func__, c->rolloff);
			return -EOPNOTSUPP;
		}
		state->dnxt.rolloff_val = CX24116_ROLLOFF_035;
		break;

	case SYS_DVBS2:
		dprintk("%s: DVB-S2 delivery system selected\n", __func__);

		/*
		 * NBC 8PSK/QPSK with DVB-S is supported for DVB-S2,
		 * but not hardware auto detection
		 */
		if (c->modulation != PSK_8 && c->modulation != QPSK) {
			dprintk("%s: unsupported modulation selected (%d)\n",
				__func__, c->modulation);
			return -EOPNOTSUPP;
		}

		switch (c->pilot) {
		case PILOT_AUTO:	/* Not supported but emulated */
			state->dnxt.pilot_val = (c->modulation == QPSK)
				? CX24116_PILOT_OFF : CX24116_PILOT_ON;
			retune++;
			break;
		case PILOT_OFF:
			state->dnxt.pilot_val = CX24116_PILOT_OFF;
			break;
		case PILOT_ON:
			state->dnxt.pilot_val = CX24116_PILOT_ON;
			break;
		default:
			dprintk("%s: unsupported pilot mode selected (%d)\n",
				__func__, c->pilot);
			return -EOPNOTSUPP;
		}

		switch (c->rolloff) {
		case ROLLOFF_20:
			state->dnxt.rolloff_val = CX24116_ROLLOFF_020;
			break;
		case ROLLOFF_25:
			state->dnxt.rolloff_val = CX24116_ROLLOFF_025;
			break;
		case ROLLOFF_35:
			state->dnxt.rolloff_val = CX24116_ROLLOFF_035;
			break;
		case ROLLOFF_AUTO:	/* Rolloff must be explicit */
		default:
			dprintk("%s: unsupported rolloff selected (%d)\n",
				__func__, c->rolloff);
			return -EOPNOTSUPP;
		}
		break;

	default:
		dprintk("%s: unsupported delivery system selected (%d)\n",
			__func__, c->delivery_system);
		return -EOPNOTSUPP;
	}
	state->dnxt.delsys = c->delivery_system;
	state->dnxt.modulation = c->modulation;
	state->dnxt.frequency = c->frequency;
	state->dnxt.pilot = c->pilot;
	state->dnxt.rolloff = c->rolloff;

	ret = cx24116_set_inversion(state, c->inversion);
	if (ret !=  0)
		return ret;

	/* FEC_NONE/AUTO for DVB-S2 is not supported and detected here */
	ret = cx24116_set_fec(state, c->delivery_system, c->modulation, c->fec_inner);
	if (ret !=  0)
		return ret;

	ret = cx24116_set_symbolrate(state, c->symbol_rate);
	if (ret !=  0)
		return ret;

	/* discard the 'current' tuning parameters and prepare to tune */
	cx24116_clone_params(fe);

	dprintk("%s:   delsys      = %d\n", __func__, state->dcur.delsys);
	dprintk("%s:   modulation  = %d\n", __func__, state->dcur.modulation);
	dprintk("%s:   frequency   = %d\n", __func__, state->dcur.frequency);
	dprintk("%s:   pilot       = %d (val = 0x%02x)\n", __func__,
		state->dcur.pilot, state->dcur.pilot_val);
	dprintk("%s:   retune      = %d\n", __func__, retune);
	dprintk("%s:   rolloff     = %d (val = 0x%02x)\n", __func__,
		state->dcur.rolloff, state->dcur.rolloff_val);
	dprintk("%s:   symbol_rate = %d\n", __func__, state->dcur.symbol_rate);
	dprintk("%s:   FEC         = %d (mask/val = 0x%02x/0x%02x)\n", __func__,
		state->dcur.fec, state->dcur.fec_mask, state->dcur.fec_val);
	dprintk("%s:   Inversion   = %d (val = 0x%02x)\n", __func__,
		state->dcur.inversion, state->dcur.inversion_val);

	/* This is also done in advise/acquire on HVR4000 but not on LITE */
	if (state->config->set_ts_params)
		state->config->set_ts_params(fe, 0);

	/* Set/Reset B/W */
	cmd.args[0x00] = CMD_BANDWIDTH;
	cmd.args[0x01] = 0x01;
	cmd.len = 0x02;
	ret = cx24116_cmd_execute(fe, &cmd);
	if (ret != 0)
		return ret;

	/* Prepare a tune request */
	cmd.args[0x00] = CMD_TUNEREQUEST;

	/* Frequency */
	cmd.args[0x01] = (state->dcur.frequency & 0xff0000) >> 16;
	cmd.args[0x02] = (state->dcur.frequency & 0x00ff00) >> 8;
	cmd.args[0x03] = (state->dcur.frequency & 0x0000ff);

	/* Symbol Rate */
	cmd.args[0x04] = ((state->dcur.symbol_rate / 1000) & 0xff00) >> 8;
	cmd.args[0x05] = ((state->dcur.symbol_rate / 1000) & 0x00ff);

	/* Automatic Inversion */
	cmd.args[0x06] = state->dcur.inversion_val;

	/* Modulation / FEC / Pilot */
	cmd.args[0x07] = state->dcur.fec_val | state->dcur.pilot_val;

	cmd.args[0x08] = CX24116_SEARCH_RANGE_KHZ >> 8;
	cmd.args[0x09] = CX24116_SEARCH_RANGE_KHZ & 0xff;
	cmd.args[0x0a] = 0x00;
	cmd.args[0x0b] = 0x00;
	cmd.args[0x0c] = state->dcur.rolloff_val;
	cmd.args[0x0d] = state->dcur.fec_mask;

	if (state->dcur.symbol_rate > 30000000) {
		cmd.args[0x0e] = 0x04;
		cmd.args[0x0f] = 0x00;
		cmd.args[0x10] = 0x01;
		cmd.args[0x11] = 0x77;
		cmd.args[0x12] = 0x36;
		cx24116_writereg(state, CX24116_REG_CLKDIV, 0x44);
		cx24116_writereg(state, CX24116_REG_RATEDIV, 0x01);
	} else {
		cmd.args[0x0e] = 0x06;
		cmd.args[0x0f] = 0x00;
		cmd.args[0x10] = 0x00;
		cmd.args[0x11] = 0xFA;
		cmd.args[0x12] = 0x24;
		cx24116_writereg(state, CX24116_REG_CLKDIV, 0x46);
		cx24116_writereg(state, CX24116_REG_RATEDIV, 0x00);
	}

	cmd.len = 0x13;

	/* We need to support pilot and non-pilot tuning in the
	 * driver automatically. This is a workaround for because
	 * the demod does not support autodetect.
	 */
	do {
		/* Reset status register */
		status = cx24116_readreg(state, CX24116_REG_SSTATUS)
			& CX24116_SIGNAL_MASK;
		cx24116_writereg(state, CX24116_REG_SSTATUS, status);

		/* Tune */
		ret = cx24116_cmd_execute(fe, &cmd);
		if (ret != 0)
			break;

		/*
		 * Wait for up to 500 ms before retrying
		 *
		 * If we are able to tune then generally it occurs within 100ms.
		 * If it takes longer, try a different toneburst setting.
		 */
		for (i = 0; i < 50 ; i++) {
			cx24116_read_status(fe, &tunerstat);
			status = tunerstat & (FE_HAS_SIGNAL | FE_HAS_SYNC);
			if (status == (FE_HAS_SIGNAL | FE_HAS_SYNC)) {
				dprintk("%s: Tuned\n", __func__);
				goto tuned;
			}
			msleep(10);
		}

		dprintk("%s: Not tuned\n", __func__);

		/* Toggle pilot bit when in auto-pilot */
		if (state->dcur.pilot == PILOT_AUTO)
			cmd.args[0x07] ^= CX24116_PILOT_ON;
	} while (--retune);

tuned:  /* Set/Reset B/W */
	cmd.args[0x00] = CMD_BANDWIDTH;
	cmd.args[0x01] = 0x00;
	cmd.len = 0x02;
	return cx24116_cmd_execute(fe, &cmd);
}

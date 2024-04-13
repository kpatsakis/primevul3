static int cx24116_diseqc_init(struct dvb_frontend *fe)
{
	struct cx24116_state *state = fe->demodulator_priv;
	struct cx24116_cmd cmd;
	int ret;

	/* Firmware CMD 20: LNB/DiSEqC config */
	cmd.args[0x00] = CMD_LNBCONFIG;
	cmd.args[0x01] = 0x00;
	cmd.args[0x02] = 0x10;
	cmd.args[0x03] = 0x00;
	cmd.args[0x04] = 0x8f;
	cmd.args[0x05] = 0x28;
	cmd.args[0x06] = (toneburst == CX24116_DISEQC_TONEOFF) ? 0x00 : 0x01;
	cmd.args[0x07] = 0x01;
	cmd.len = 0x08;
	ret = cx24116_cmd_execute(fe, &cmd);
	if (ret != 0)
		return ret;

	/* Prepare a DiSEqC command */
	state->dsec_cmd.args[0x00] = CMD_LNBSEND;

	/* DiSEqC burst */
	state->dsec_cmd.args[CX24116_DISEQC_BURST]  = CX24116_DISEQC_MINI_A;

	/* Unknown */
	state->dsec_cmd.args[CX24116_DISEQC_ARG2_2] = 0x02;
	state->dsec_cmd.args[CX24116_DISEQC_ARG3_0] = 0x00;
	/* Continuation flag? */
	state->dsec_cmd.args[CX24116_DISEQC_ARG4_0] = 0x00;

	/* DiSEqC message length */
	state->dsec_cmd.args[CX24116_DISEQC_MSGLEN] = 0x00;

	/* Command length */
	state->dsec_cmd.len = CX24116_DISEQC_MSGOFS;

	return 0;
}

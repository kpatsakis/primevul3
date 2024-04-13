static void cx24116_clone_params(struct dvb_frontend *fe)
{
	struct cx24116_state *state = fe->demodulator_priv;
	state->dcur = state->dnxt;
}

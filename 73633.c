static int cx24116_set_voltage(struct dvb_frontend *fe,
	fe_sec_voltage_t voltage)
{
	struct cx24116_cmd cmd;
	int ret;

	dprintk("%s: %s\n", __func__,
		voltage == SEC_VOLTAGE_13 ? "SEC_VOLTAGE_13" :
		voltage == SEC_VOLTAGE_18 ? "SEC_VOLTAGE_18" : "??");

	/* Wait for LNB ready */
	ret = cx24116_wait_for_lnb(fe);
	if (ret != 0)
		return ret;

	/* Wait for voltage/min repeat delay */
	msleep(100);

	cmd.args[0x00] = CMD_LNBDCLEVEL;
	cmd.args[0x01] = (voltage == SEC_VOLTAGE_18 ? 0x01 : 0x00);
	cmd.len = 0x02;

	/* Min delay time before DiSEqC send */
	msleep(15);

	return cx24116_cmd_execute(fe, &cmd);
}

static int cx24116_set_tone(struct dvb_frontend *fe,
	fe_sec_tone_mode_t tone)
{
	struct cx24116_cmd cmd;
	int ret;

	dprintk("%s(%d)\n", __func__, tone);
	if ((tone != SEC_TONE_ON) && (tone != SEC_TONE_OFF)) {
		printk(KERN_ERR "%s: Invalid, tone=%d\n", __func__, tone);
		return -EINVAL;
	}

	/* Wait for LNB ready */
	ret = cx24116_wait_for_lnb(fe);
	if (ret != 0)
		return ret;

	/* Min delay time after DiSEqC send */
	msleep(15); /* XXX determine is FW does this, see send_diseqc/burst */

	/* Now we set the tone */
	cmd.args[0x00] = CMD_SET_TONE;
	cmd.args[0x01] = 0x00;
	cmd.args[0x02] = 0x00;

	switch (tone) {
	case SEC_TONE_ON:
		dprintk("%s: setting tone on\n", __func__);
		cmd.args[0x03] = 0x01;
		break;
	case SEC_TONE_OFF:
		dprintk("%s: setting tone off\n", __func__);
		cmd.args[0x03] = 0x00;
		break;
	}
	cmd.len = 0x04;

	/* Min delay time before DiSEqC send */
	msleep(15); /* XXX determine is FW does this, see send_diseqc/burst */

	return cx24116_cmd_execute(fe, &cmd);
}

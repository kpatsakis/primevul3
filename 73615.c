static int cx24116_firmware_ondemand(struct dvb_frontend *fe)
{
	struct cx24116_state *state = fe->demodulator_priv;
	const struct firmware *fw;
	int ret = 0;

	dprintk("%s()\n", __func__);

	if (cx24116_readreg(state, 0x20) > 0) {

		if (state->skip_fw_load)
			return 0;

		/* Load firmware */
		/* request the firmware, this will block until loaded */
		printk(KERN_INFO "%s: Waiting for firmware upload (%s)...\n",
			__func__, CX24116_DEFAULT_FIRMWARE);
		ret = request_firmware(&fw, CX24116_DEFAULT_FIRMWARE,
			state->i2c->dev.parent);
		printk(KERN_INFO "%s: Waiting for firmware upload(2)...\n",
			__func__);
		if (ret) {
			printk(KERN_ERR "%s: No firmware uploaded "
				"(timeout or file not found?)\n", __func__);
			return ret;
		}

		/* Make sure we don't recurse back through here
		 * during loading */
		state->skip_fw_load = 1;

		ret = cx24116_load_firmware(fe, fw);
		if (ret)
			printk(KERN_ERR "%s: Writing firmware to device failed\n",
				__func__);

		release_firmware(fw);

		printk(KERN_INFO "%s: Firmware upload %s\n", __func__,
			ret == 0 ? "complete" : "failed");

		/* Ensure firmware is always loaded if required */
		state->skip_fw_load = 0;
	}

	return ret;
}

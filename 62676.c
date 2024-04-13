static unsigned long cmd_input_size(unsigned int cmd)
{
	/* Size of structure up to and including 'field' */
#define CMDINSIZE(cmd, type, field) 				\
	case VIDIOC_##cmd: 					\
		return offsetof(struct v4l2_##type, field) + 	\
			sizeof(((struct v4l2_##type *)0)->field);

	switch (cmd) {
		CMDINSIZE(ENUM_FMT,		fmtdesc,	type);
		CMDINSIZE(G_FMT,		format,		type);
		CMDINSIZE(QUERYBUF,		buffer,		length);
		CMDINSIZE(G_PARM,		streamparm,	type);
		CMDINSIZE(ENUMSTD,		standard,	index);
		CMDINSIZE(ENUMINPUT,		input,		index);
		CMDINSIZE(G_CTRL,		control,	id);
		CMDINSIZE(G_TUNER,		tuner,		index);
		CMDINSIZE(QUERYCTRL,		queryctrl,	id);
		CMDINSIZE(QUERYMENU,		querymenu,	index);
		CMDINSIZE(ENUMOUTPUT,		output,		index);
		CMDINSIZE(G_MODULATOR,		modulator,	index);
		CMDINSIZE(G_FREQUENCY,		frequency,	tuner);
		CMDINSIZE(CROPCAP,		cropcap,	type);
		CMDINSIZE(G_CROP,		crop,		type);
		CMDINSIZE(ENUMAUDIO,		audio, 		index);
		CMDINSIZE(ENUMAUDOUT,		audioout, 	index);
		CMDINSIZE(ENCODER_CMD,		encoder_cmd,	flags);
		CMDINSIZE(TRY_ENCODER_CMD,	encoder_cmd,	flags);
		CMDINSIZE(G_SLICED_VBI_CAP,	sliced_vbi_cap,	type);
		CMDINSIZE(ENUM_FRAMESIZES,	frmsizeenum,	pixel_format);
		CMDINSIZE(ENUM_FRAMEINTERVALS,	frmivalenum,	height);
	default:
		return _IOC_SIZE(cmd);
	}
}

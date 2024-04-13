tsize_t t2p_readwrite_pdf_image(T2P* t2p, TIFF* input, TIFF* output){

	tsize_t written=0;
	unsigned char* buffer=NULL;
	unsigned char* samplebuffer=NULL;
	tsize_t bufferoffset=0;
	tsize_t samplebufferoffset=0;
	tsize_t read=0;
	tstrip_t i=0;
	tstrip_t j=0;
	tstrip_t stripcount=0;
	tsize_t stripsize=0;
	tsize_t sepstripcount=0;
	tsize_t sepstripsize=0;
#ifdef OJPEG_SUPPORT
	toff_t inputoffset=0;
	uint16 h_samp=1;
	uint16 v_samp=1;
	uint16 ri=1;
	uint32 rows=0;
#endif /* ifdef OJPEG_SUPPORT */
#ifdef JPEG_SUPPORT
	unsigned char* jpt;
	float* xfloatp;
	uint64* sbc;
	unsigned char* stripbuffer;
	tsize_t striplength=0;
	uint32 max_striplength=0;
#endif /* ifdef JPEG_SUPPORT */

	/* Fail if prior error (in particular, can't trust tiff_datasize) */
	if (t2p->t2p_error != T2P_ERR_OK)
		return(0);

	if(t2p->pdf_transcode == T2P_TRANSCODE_RAW){
#ifdef CCITT_SUPPORT
		if(t2p->pdf_compression == T2P_COMPRESS_G4){
			buffer = (unsigned char*)
				_TIFFmalloc(t2p->tiff_datasize);
			if (buffer == NULL) {
				TIFFError(TIFF2PDF_MODULE, 
                                          "Can't allocate %lu bytes of memory for "
                                          "t2p_readwrite_pdf_image, %s", 
					(unsigned long) t2p->tiff_datasize, 
					TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
				return(0);
			}
			TIFFReadRawStrip(input, 0, (tdata_t) buffer,
					 t2p->tiff_datasize);
			if (t2p->tiff_fillorder==FILLORDER_LSB2MSB){
					/*
					 * make sure is lsb-to-msb
					 * bit-endianness fill order
					 */
					TIFFReverseBits(buffer,
							t2p->tiff_datasize);
			}
			t2pWriteFile(output, (tdata_t) buffer,
				      t2p->tiff_datasize);
			_TIFFfree(buffer);
			return(t2p->tiff_datasize);
		}
#endif /* ifdef CCITT_SUPPORT */
#ifdef ZIP_SUPPORT
		if (t2p->pdf_compression == T2P_COMPRESS_ZIP) {
			buffer = (unsigned char*)
				_TIFFmalloc(t2p->tiff_datasize);
			if(buffer == NULL){
				TIFFError(TIFF2PDF_MODULE, 
	"Can't allocate %lu bytes of memory for t2p_readwrite_pdf_image, %s", 
					(unsigned long) t2p->tiff_datasize, 
					TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
				return(0);
			}
                        memset(buffer, 0, t2p->tiff_datasize);
			TIFFReadRawStrip(input, 0, (tdata_t) buffer,
					 t2p->tiff_datasize);
			if (t2p->tiff_fillorder==FILLORDER_LSB2MSB) {
					TIFFReverseBits(buffer,
							t2p->tiff_datasize);
			}
			t2pWriteFile(output, (tdata_t) buffer,
				      t2p->tiff_datasize);
			_TIFFfree(buffer);
			return(t2p->tiff_datasize);
		}
#endif /* ifdef ZIP_SUPPORT */
#ifdef OJPEG_SUPPORT
		if(t2p->tiff_compression == COMPRESSION_OJPEG) {

			if(t2p->tiff_dataoffset != 0) {
				buffer = (unsigned char*)
					_TIFFmalloc(t2p->tiff_datasize);
				if(buffer == NULL) {
					TIFFError(TIFF2PDF_MODULE, 
	"Can't allocate %lu bytes of memory for t2p_readwrite_pdf_image, %s", 
						(unsigned long) t2p->tiff_datasize, 
						TIFFFileName(input));
					t2p->t2p_error = T2P_ERR_ERROR;
					return(0);
				}
                                memset(buffer, 0, t2p->tiff_datasize);
				if(t2p->pdf_ojpegiflength==0){
					inputoffset=t2pSeekFile(input, 0,
								 SEEK_CUR);
					t2pSeekFile(input,
						     t2p->tiff_dataoffset,
						     SEEK_SET);
					t2pReadFile(input, (tdata_t) buffer,
						     t2p->tiff_datasize);
					t2pSeekFile(input, inputoffset,
						     SEEK_SET);
					t2pWriteFile(output, (tdata_t) buffer,
						      t2p->tiff_datasize);
					_TIFFfree(buffer);
					return(t2p->tiff_datasize);
				} else {
					inputoffset=t2pSeekFile(input, 0,
								 SEEK_CUR);
					t2pSeekFile(input,
						     t2p->tiff_dataoffset,
						     SEEK_SET);
					bufferoffset = t2pReadFile(input,
						(tdata_t) buffer,
						t2p->pdf_ojpegiflength);
					t2p->pdf_ojpegiflength = 0;
					t2pSeekFile(input, inputoffset,
						     SEEK_SET);
					TIFFGetField(input,
						     TIFFTAG_YCBCRSUBSAMPLING,
						     &h_samp, &v_samp);
					buffer[bufferoffset++]= 0xff;
					buffer[bufferoffset++]= 0xdd;
					buffer[bufferoffset++]= 0x00;
					buffer[bufferoffset++]= 0x04;
					h_samp*=8;
					v_samp*=8;
					ri=(t2p->tiff_width+h_samp-1) / h_samp;
					TIFFGetField(input,
						     TIFFTAG_ROWSPERSTRIP,
						     &rows);
					ri*=(rows+v_samp-1)/v_samp;
					buffer[bufferoffset++]= (ri>>8) & 0xff;
					buffer[bufferoffset++]= ri & 0xff;
					stripcount=TIFFNumberOfStrips(input);
					for(i=0;i<stripcount;i++){
						if(i != 0 ){ 
							buffer[bufferoffset++]=0xff;
							buffer[bufferoffset++]=(0xd0 | ((i-1)%8));
						}
						bufferoffset+=TIFFReadRawStrip(input, 
							i, 
							(tdata_t) &(((unsigned char*)buffer)[bufferoffset]), 
							-1);
					}
					t2pWriteFile(output, (tdata_t) buffer, bufferoffset);
					_TIFFfree(buffer);
					return(bufferoffset);
				}
			} else {
				if(! t2p->pdf_ojpegdata){
					TIFFError(TIFF2PDF_MODULE, 
				"No support for OJPEG image %s with bad tables", 
						TIFFFileName(input));
					t2p->t2p_error = T2P_ERR_ERROR;
					return(0);
				}
				buffer = (unsigned char*)
					_TIFFmalloc(t2p->tiff_datasize);
				if(buffer==NULL){
					TIFFError(TIFF2PDF_MODULE, 
	"Can't allocate %lu bytes of memory for t2p_readwrite_pdf_image, %s", 
						(unsigned long) t2p->tiff_datasize, 
						TIFFFileName(input));
					t2p->t2p_error = T2P_ERR_ERROR;
					return(0);
				}
                                memset(buffer, 0, t2p->tiff_datasize);
				_TIFFmemcpy(buffer, t2p->pdf_ojpegdata, t2p->pdf_ojpegdatalength);
				bufferoffset=t2p->pdf_ojpegdatalength;
				stripcount=TIFFNumberOfStrips(input);
				for(i=0;i<stripcount;i++){
					if(i != 0){
						buffer[bufferoffset++]=0xff;
						buffer[bufferoffset++]=(0xd0 | ((i-1)%8));
					}
					bufferoffset+=TIFFReadRawStrip(input, 
						i, 
						(tdata_t) &(((unsigned char*)buffer)[bufferoffset]), 
						-1);
				}
				if( ! ( (buffer[bufferoffset-1]==0xd9) && (buffer[bufferoffset-2]==0xff) ) ){
						buffer[bufferoffset++]=0xff;
						buffer[bufferoffset++]=0xd9;
				}
				t2pWriteFile(output, (tdata_t) buffer, bufferoffset);
				_TIFFfree(buffer);
				return(bufferoffset);
#if 0
                                /*
                                  This hunk of code removed code is clearly
                                  mis-placed and we are not sure where it
                                  should be (if anywhere)
                                */
				TIFFError(TIFF2PDF_MODULE, 
	"No support for OJPEG image %s with no JPEG File Interchange offset", 
					TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
				return(0);
#endif
			}
		}
#endif /* ifdef OJPEG_SUPPORT */
#ifdef JPEG_SUPPORT
		if(t2p->tiff_compression == COMPRESSION_JPEG) {
			uint32 count = 0;
			buffer = (unsigned char*)
				_TIFFmalloc(t2p->tiff_datasize);
			if(buffer==NULL){
				TIFFError(TIFF2PDF_MODULE, 
	"Can't allocate %lu bytes of memory for t2p_readwrite_pdf_image, %s", 
					(unsigned long) t2p->tiff_datasize, 
					TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
				return(0);
			}
                        memset(buffer, 0, t2p->tiff_datasize);
			if (TIFFGetField(input, TIFFTAG_JPEGTABLES, &count, &jpt) != 0) {
				if(count > 4) {
					_TIFFmemcpy(buffer, jpt, count);
					bufferoffset += count - 2;
				}
			}
			stripcount=TIFFNumberOfStrips(input);
			TIFFGetField(input, TIFFTAG_STRIPBYTECOUNTS, &sbc);
			for(i=0;i<stripcount;i++){
				if(sbc[i]>max_striplength) max_striplength=sbc[i];
			}
			stripbuffer = (unsigned char*)
				_TIFFmalloc(max_striplength);
			if(stripbuffer==NULL){
				TIFFError(TIFF2PDF_MODULE, 
	"Can't allocate %u bytes of memory for t2p_readwrite_pdf_image, %s", 
					max_striplength, 
					TIFFFileName(input));
				_TIFFfree(buffer);
				t2p->t2p_error = T2P_ERR_ERROR;
				return(0);
			}
			for(i=0;i<stripcount;i++){
				striplength=TIFFReadRawStrip(input, i, (tdata_t) stripbuffer, -1);
				if(!t2p_process_jpeg_strip(
					stripbuffer, 
					&striplength, 
					buffer,
                    t2p->tiff_datasize,
					&bufferoffset, 
					i, 
					t2p->tiff_length)){
						TIFFError(TIFF2PDF_MODULE, 
				"Can't process JPEG data in input file %s", 
							TIFFFileName(input));
						_TIFFfree(samplebuffer);
						_TIFFfree(buffer);
						t2p->t2p_error = T2P_ERR_ERROR;
						return(0);
				}
			}
			buffer[bufferoffset++]=0xff; 
			buffer[bufferoffset++]=0xd9;
			t2pWriteFile(output, (tdata_t) buffer, bufferoffset);
			_TIFFfree(stripbuffer);
			_TIFFfree(buffer);
			return(bufferoffset);
		}
#endif /* ifdef JPEG_SUPPORT */
		(void)0;
	}

	if(t2p->pdf_sample==T2P_SAMPLE_NOTHING){
		buffer = (unsigned char*) _TIFFmalloc(t2p->tiff_datasize);
		if(buffer==NULL){
			TIFFError(TIFF2PDF_MODULE, 
	"Can't allocate %lu bytes of memory for t2p_readwrite_pdf_image, %s", 
				(unsigned long) t2p->tiff_datasize, 
				TIFFFileName(input));
			t2p->t2p_error = T2P_ERR_ERROR;
			return(0);
		}
                memset(buffer, 0, t2p->tiff_datasize);
		stripsize=TIFFStripSize(input);
		stripcount=TIFFNumberOfStrips(input);
		for(i=0;i<stripcount;i++){
			read = 
				TIFFReadEncodedStrip(input, 
				i, 
				(tdata_t) &buffer[bufferoffset], 
				TIFFmin(stripsize, t2p->tiff_datasize - bufferoffset));
			if(read==-1){
				TIFFError(TIFF2PDF_MODULE, 
					"Error on decoding strip %u of %s", 
					i, 
					TIFFFileName(input));
				_TIFFfree(buffer);
				t2p->t2p_error=T2P_ERR_ERROR;
				return(0);
			}
			bufferoffset+=read;
		}
	} else {
		if(t2p->pdf_sample & T2P_SAMPLE_PLANAR_SEPARATE_TO_CONTIG){
		
			sepstripsize=TIFFStripSize(input);
			sepstripcount=TIFFNumberOfStrips(input);
		
			stripsize=sepstripsize*t2p->tiff_samplesperpixel;
			stripcount=sepstripcount/t2p->tiff_samplesperpixel;
			
			buffer = (unsigned char*) _TIFFmalloc(t2p->tiff_datasize);
			if(buffer==NULL){
				TIFFError(TIFF2PDF_MODULE, 
	"Can't allocate %lu bytes of memory for t2p_readwrite_pdf_image, %s", 
					(unsigned long) t2p->tiff_datasize, 
					TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
				return(0);
			}
                        memset(buffer, 0, t2p->tiff_datasize);
			samplebuffer = (unsigned char*) _TIFFmalloc(stripsize);
			if(samplebuffer==NULL){
				TIFFError(TIFF2PDF_MODULE, 
	"Can't allocate %lu bytes of memory for t2p_readwrite_pdf_image, %s", 
					(unsigned long) t2p->tiff_datasize, 
					TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
                                _TIFFfree(buffer);
				return(0);
			}
			for(i=0;i<stripcount;i++){
				samplebufferoffset=0;
				for(j=0;j<t2p->tiff_samplesperpixel;j++){
					read = 
						TIFFReadEncodedStrip(input, 
							i + j*stripcount, 
							(tdata_t) &(samplebuffer[samplebufferoffset]), 
							TIFFmin(sepstripsize, stripsize - samplebufferoffset));
					if(read==-1){
						TIFFError(TIFF2PDF_MODULE, 
					"Error on decoding strip %u of %s", 
							i + j*stripcount, 
							TIFFFileName(input));
							_TIFFfree(buffer);
						t2p->t2p_error=T2P_ERR_ERROR;
						return(0);
					}
					samplebufferoffset+=read;
				}
				t2p_sample_planar_separate_to_contig(
					t2p,
					&(buffer[bufferoffset]),
					samplebuffer, 
					samplebufferoffset); 
				bufferoffset+=samplebufferoffset;
			}
			_TIFFfree(samplebuffer);
			goto dataready;
		}

		buffer = (unsigned char*) _TIFFmalloc(t2p->tiff_datasize);
		if(buffer==NULL){
			TIFFError(TIFF2PDF_MODULE, 
	"Can't allocate %lu bytes of memory for t2p_readwrite_pdf_image, %s", 
				(unsigned long) t2p->tiff_datasize, 
				TIFFFileName(input));
			t2p->t2p_error = T2P_ERR_ERROR;
			return(0);
		}
                memset(buffer, 0, t2p->tiff_datasize);
		stripsize=TIFFStripSize(input);
		stripcount=TIFFNumberOfStrips(input);
		for(i=0;i<stripcount;i++){
			read = 
				TIFFReadEncodedStrip(input, 
				i, 
				(tdata_t) &buffer[bufferoffset], 
				TIFFmin(stripsize, t2p->tiff_datasize - bufferoffset));
			if(read==-1){
				TIFFError(TIFF2PDF_MODULE, 
					"Error on decoding strip %u of %s", 
					i, 
					TIFFFileName(input));
				_TIFFfree(samplebuffer);
				_TIFFfree(buffer);
				t2p->t2p_error=T2P_ERR_ERROR;
				return(0);
			}
			bufferoffset+=read;
		}

		if(t2p->pdf_sample & T2P_SAMPLE_REALIZE_PALETTE){
			samplebuffer=(unsigned char*)_TIFFrealloc( 
				(tdata_t) buffer, 
				t2p->tiff_datasize * t2p->tiff_samplesperpixel);
			if(samplebuffer==NULL){
				TIFFError(TIFF2PDF_MODULE, 
	"Can't allocate %lu bytes of memory for t2p_readwrite_pdf_image, %s", 
					(unsigned long) t2p->tiff_datasize, 
					TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
				_TIFFfree(buffer);
				return(0);
			} else {
				buffer=samplebuffer;
				t2p->tiff_datasize *= t2p->tiff_samplesperpixel;
			}
			t2p_sample_realize_palette(t2p, buffer);
		}

		if(t2p->pdf_sample & T2P_SAMPLE_RGBA_TO_RGB){
			t2p->tiff_datasize=t2p_sample_rgba_to_rgb(
				(tdata_t)buffer, 
				t2p->tiff_width*t2p->tiff_length);
		}

		if(t2p->pdf_sample & T2P_SAMPLE_RGBAA_TO_RGB){
			t2p->tiff_datasize=t2p_sample_rgbaa_to_rgb(
				(tdata_t)buffer, 
				t2p->tiff_width*t2p->tiff_length);
		}

		if(t2p->pdf_sample & T2P_SAMPLE_YCBCR_TO_RGB){
			samplebuffer=(unsigned char*)_TIFFrealloc(
				(tdata_t)buffer, 
				t2p->tiff_width*t2p->tiff_length*4);
			if(samplebuffer==NULL){
				TIFFError(TIFF2PDF_MODULE, 
	"Can't allocate %lu bytes of memory for t2p_readwrite_pdf_image, %s", 
					(unsigned long) t2p->tiff_datasize, 
					TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
				_TIFFfree(buffer);
				return(0);
			} else {
				buffer=samplebuffer;
			}
			if(!TIFFReadRGBAImageOriented(
				input, 
				t2p->tiff_width, 
				t2p->tiff_length, 
				(uint32*)buffer, 
				ORIENTATION_TOPLEFT,
				0)){
				TIFFError(TIFF2PDF_MODULE, 
	"Can't use TIFFReadRGBAImageOriented to extract RGB image from %s", 
					TIFFFileName(input));
				t2p->t2p_error = T2P_ERR_ERROR;
				return(0);
			}
			t2p->tiff_datasize=t2p_sample_abgr_to_rgb(
				(tdata_t) buffer, 
				t2p->tiff_width*t2p->tiff_length);

		}

		if(t2p->pdf_sample & T2P_SAMPLE_LAB_SIGNED_TO_UNSIGNED){
			t2p->tiff_datasize=t2p_sample_lab_signed_to_unsigned(
				(tdata_t)buffer, 
				t2p->tiff_width*t2p->tiff_length);
		}
	}

dataready:

	t2p_disable(output);
	TIFFSetField(output, TIFFTAG_PHOTOMETRIC, t2p->tiff_photometric);
	TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, t2p->tiff_bitspersample);
	TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, t2p->tiff_samplesperpixel);
	TIFFSetField(output, TIFFTAG_IMAGEWIDTH, t2p->tiff_width);
	TIFFSetField(output, TIFFTAG_IMAGELENGTH, t2p->tiff_length);
	TIFFSetField(output, TIFFTAG_ROWSPERSTRIP, t2p->tiff_length);
	TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(output, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);

	switch(t2p->pdf_compression){
	case T2P_COMPRESS_NONE:
		TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
		break;
#ifdef CCITT_SUPPORT
	case T2P_COMPRESS_G4:
		TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_CCITTFAX4);
		break;
#endif /* ifdef CCITT_SUPPORT */
#ifdef JPEG_SUPPORT
	case T2P_COMPRESS_JPEG:
		if(t2p->tiff_photometric==PHOTOMETRIC_YCBCR) {
			uint16 hor = 0, ver = 0;
			if (TIFFGetField(input, TIFFTAG_YCBCRSUBSAMPLING, &hor, &ver) !=0 ) {
				if(hor != 0 && ver != 0){
					TIFFSetField(output, TIFFTAG_YCBCRSUBSAMPLING, hor, ver);
				}
			}
			if(TIFFGetField(input, TIFFTAG_REFERENCEBLACKWHITE, &xfloatp)!=0){
				TIFFSetField(output, TIFFTAG_REFERENCEBLACKWHITE, xfloatp);
			}
		}
		if(TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_JPEG)==0){
			TIFFError(TIFF2PDF_MODULE, 
		"Unable to use JPEG compression for input %s and output %s", 
				TIFFFileName(input),
				TIFFFileName(output));
			_TIFFfree(buffer);
			t2p->t2p_error = T2P_ERR_ERROR;
			return(0);
		}
		TIFFSetField(output, TIFFTAG_JPEGTABLESMODE, 0);

		if(t2p->pdf_colorspace & (T2P_CS_RGB | T2P_CS_LAB)){
			TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_YCBCR);
			if(t2p->tiff_photometric != PHOTOMETRIC_YCBCR){
				TIFFSetField(output, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RGB);
			} else {
				TIFFSetField(output, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RAW);
			}
		}
		if(t2p->pdf_colorspace & T2P_CS_GRAY){
			(void)0;
		}
		if(t2p->pdf_colorspace & T2P_CS_CMYK){
			(void)0;
		}
		if(t2p->pdf_defaultcompressionquality != 0){
			TIFFSetField(output, 
				TIFFTAG_JPEGQUALITY, 
				t2p->pdf_defaultcompressionquality);
		}
	
		break;
#endif /* ifdef JPEG_SUPPORT */
#ifdef ZIP_SUPPORT
	case T2P_COMPRESS_ZIP:
		TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
		if(t2p->pdf_defaultcompressionquality%100 != 0){
			TIFFSetField(output, 
				TIFFTAG_PREDICTOR, 
				t2p->pdf_defaultcompressionquality % 100);
		}
		if(t2p->pdf_defaultcompressionquality/100 != 0){
			TIFFSetField(output, 
				TIFFTAG_ZIPQUALITY, 
				(t2p->pdf_defaultcompressionquality / 100));
		}
		break;
#endif /* ifdef ZIP_SUPPORT */
	default:
		break;
	}

	t2p_enable(output);
	t2p->outputwritten = 0;
#ifdef JPEG_SUPPORT
	if(t2p->pdf_compression == T2P_COMPRESS_JPEG
	   && t2p->tiff_photometric == PHOTOMETRIC_YCBCR){
		bufferoffset = TIFFWriteEncodedStrip(output, (tstrip_t)0,
						     buffer,
						     stripsize * stripcount); 
	} else
#endif /* ifdef JPEG_SUPPORT */
        {
		bufferoffset = TIFFWriteEncodedStrip(output, (tstrip_t)0,
						     buffer,
						     t2p->tiff_datasize); 
	}
	if (buffer != NULL) {
		_TIFFfree(buffer);
		buffer=NULL;
	}

	if (bufferoffset == (tsize_t)-1) {
		TIFFError(TIFF2PDF_MODULE, 
			  "Error writing encoded strip to output PDF %s", 
			  TIFFFileName(output));
		t2p->t2p_error = T2P_ERR_ERROR;
		return(0);
	}
	
	written = t2p->outputwritten;
	return(written);
}

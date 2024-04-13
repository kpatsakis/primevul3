NTSTATUS ProcessVolumeDeviceControlIrp (PDEVICE_OBJECT DeviceObject, PEXTENSION Extension, PIRP Irp)
{
	PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation (Irp);

	switch (irpSp->Parameters.DeviceIoControl.IoControlCode)
	{

	case IOCTL_MOUNTDEV_QUERY_DEVICE_NAME:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_MOUNTDEV_QUERY_DEVICE_NAME)\n");
		if (!ValidateIOBufferSize (Irp, sizeof (MOUNTDEV_NAME), ValidateOutput))
		{
			Irp->IoStatus.Information = sizeof (MOUNTDEV_NAME);
			Irp->IoStatus.Status = STATUS_BUFFER_OVERFLOW;
		}
		else
		{
			ULONG outLength;
			UNICODE_STRING ntUnicodeString;
			WCHAR ntName[256];
			PMOUNTDEV_NAME outputBuffer = (PMOUNTDEV_NAME) Irp->AssociatedIrp.SystemBuffer;

			TCGetNTNameFromNumber (ntName, sizeof(ntName),Extension->nDosDriveNo);
			RtlInitUnicodeString (&ntUnicodeString, ntName);

			outputBuffer->NameLength = ntUnicodeString.Length;
			outLength = ntUnicodeString.Length + sizeof(USHORT);

			if (irpSp->Parameters.DeviceIoControl.OutputBufferLength < outLength)
			{
				Irp->IoStatus.Information = sizeof (MOUNTDEV_NAME);
				Irp->IoStatus.Status = STATUS_BUFFER_OVERFLOW;

				break;
			}

			RtlCopyMemory ((PCHAR)outputBuffer->Name,ntUnicodeString.Buffer, ntUnicodeString.Length);

			Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = outLength;

			Dump ("name = %ls\n",ntName);
		}
		break;

	case IOCTL_MOUNTDEV_QUERY_UNIQUE_ID:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_MOUNTDEV_QUERY_UNIQUE_ID)\n");
		if (!ValidateIOBufferSize (Irp, sizeof (MOUNTDEV_UNIQUE_ID), ValidateOutput))
		{
			Irp->IoStatus.Information = sizeof (MOUNTDEV_UNIQUE_ID);
			Irp->IoStatus.Status = STATUS_BUFFER_OVERFLOW;
		}
		else
		{
			ULONG outLength;
			UCHAR volId[128], tmp[] = { 0,0 };
			PMOUNTDEV_UNIQUE_ID outputBuffer = (PMOUNTDEV_UNIQUE_ID) Irp->AssociatedIrp.SystemBuffer;

			RtlStringCbCopyA (volId, sizeof(volId),TC_UNIQUE_ID_PREFIX);
			tmp[0] = 'A' + (UCHAR) Extension->nDosDriveNo;
			RtlStringCbCatA (volId, sizeof(volId),tmp);

			outputBuffer->UniqueIdLength = (USHORT) strlen (volId);
			outLength = (ULONG) (strlen (volId) + sizeof (USHORT));

			if (irpSp->Parameters.DeviceIoControl.OutputBufferLength < outLength)
			{
				Irp->IoStatus.Information = sizeof (MOUNTDEV_UNIQUE_ID);
				Irp->IoStatus.Status = STATUS_BUFFER_OVERFLOW;
				break;
			}

			RtlCopyMemory ((PCHAR)outputBuffer->UniqueId, volId, strlen (volId));

			Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = outLength;

			Dump ("id = %s\n",volId);
		}
		break;

	case IOCTL_MOUNTDEV_QUERY_SUGGESTED_LINK_NAME:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_MOUNTDEV_QUERY_SUGGESTED_LINK_NAME)\n");
		{
			ULONG outLength;
			UNICODE_STRING ntUnicodeString;
			WCHAR ntName[256];
			PMOUNTDEV_SUGGESTED_LINK_NAME outputBuffer = (PMOUNTDEV_SUGGESTED_LINK_NAME) Irp->AssociatedIrp.SystemBuffer;

			if (!ValidateIOBufferSize (Irp, sizeof (MOUNTDEV_SUGGESTED_LINK_NAME), ValidateOutput))
			{
				Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
				Irp->IoStatus.Information = 0;
				break;
			}

			TCGetDosNameFromNumber (ntName, sizeof(ntName),Extension->nDosDriveNo, DeviceNamespaceDefault);
			RtlInitUnicodeString (&ntUnicodeString, ntName);

			outLength = FIELD_OFFSET(MOUNTDEV_SUGGESTED_LINK_NAME,Name) + ntUnicodeString.Length;

			outputBuffer->UseOnlyIfThereAreNoOtherLinks = FALSE;
			outputBuffer->NameLength = ntUnicodeString.Length;

			if(irpSp->Parameters.DeviceIoControl.OutputBufferLength < outLength)
			{
				Irp->IoStatus.Information = sizeof (MOUNTDEV_SUGGESTED_LINK_NAME);
				Irp->IoStatus.Status = STATUS_BUFFER_OVERFLOW;
				break;
			}

			RtlCopyMemory ((PCHAR)outputBuffer->Name,ntUnicodeString.Buffer, ntUnicodeString.Length);

			Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = outLength;

			Dump ("link = %ls\n",ntName);
		}
		break;

	case IOCTL_DISK_GET_MEDIA_TYPES:
	case IOCTL_DISK_GET_DRIVE_GEOMETRY:
	case IOCTL_STORAGE_GET_MEDIA_TYPES:
	case IOCTL_DISK_UPDATE_DRIVE_SIZE:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_DISK_GET_DRIVE_GEOMETRY)\n");
		/* Return the drive geometry for the disk.  Note that we
		   return values which were made up to suit the disk size.  */
		if (ValidateIOBufferSize (Irp, sizeof (DISK_GEOMETRY), ValidateOutput))
		{
			PDISK_GEOMETRY outputBuffer = (PDISK_GEOMETRY)
			Irp->AssociatedIrp.SystemBuffer;

			outputBuffer->MediaType = Extension->bRemovable ? RemovableMedia : FixedMedia;
			outputBuffer->Cylinders.QuadPart = Extension->NumberOfCylinders;
			outputBuffer->TracksPerCylinder = Extension->TracksPerCylinder;
			outputBuffer->SectorsPerTrack = Extension->SectorsPerTrack;
			outputBuffer->BytesPerSector = Extension->BytesPerSector;
			Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = sizeof (DISK_GEOMETRY);
		}
		break;

	case IOCTL_DISK_GET_DRIVE_GEOMETRY_EX:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_DISK_GET_DRIVE_GEOMETRY_EX)\n");
		{
			ULONG minOutputSize = IsOSAtLeast (WIN_SERVER_2003)? sizeof (DISK_GEOMETRY_EX) : sizeof (DISK_GEOMETRY) + sizeof (LARGE_INTEGER);
			ULONG fullOutputSize = sizeof (DISK_GEOMETRY) + sizeof (LARGE_INTEGER) + sizeof (DISK_PARTITION_INFO) + sizeof (DISK_DETECTION_INFO);

			if (ValidateIOBufferSize (Irp, minOutputSize, ValidateOutput))
			{
				PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation (Irp);
				BOOL bFullBuffer = (irpSp->Parameters.DeviceIoControl.OutputBufferLength >= fullOutputSize)? TRUE : FALSE;
				PDISK_GEOMETRY_EX outputBuffer = (PDISK_GEOMETRY_EX) Irp->AssociatedIrp.SystemBuffer;

				outputBuffer->Geometry.MediaType = Extension->bRemovable ? RemovableMedia : FixedMedia;
				outputBuffer->Geometry.Cylinders.QuadPart = Extension->NumberOfCylinders;
				outputBuffer->Geometry.TracksPerCylinder = Extension->TracksPerCylinder;
				outputBuffer->Geometry.SectorsPerTrack = Extension->SectorsPerTrack;
				outputBuffer->Geometry.BytesPerSector = Extension->BytesPerSector;
				/* add one sector to DiskLength since our partition size is DiskLength and its offset if BytesPerSector */
				outputBuffer->DiskSize.QuadPart = Extension->DiskLength + Extension->BytesPerSector;

				if (bFullBuffer)
				{
					PDISK_PARTITION_INFO pPartInfo = (PDISK_PARTITION_INFO)(((ULONG_PTR) outputBuffer) + sizeof (DISK_GEOMETRY) + sizeof (LARGE_INTEGER));
					PDISK_DETECTION_INFO pDetectInfo = ((PDISK_DETECTION_INFO)((((ULONG_PTR) pPartInfo) + sizeof (DISK_PARTITION_INFO))));

					pPartInfo->SizeOfPartitionInfo = sizeof (DISK_PARTITION_INFO);
					pPartInfo->PartitionStyle = PARTITION_STYLE_MBR;
					pPartInfo->Mbr.Signature = GetCrc32((unsigned char*) &(Extension->UniqueVolumeId), 4);

					pDetectInfo->SizeOfDetectInfo = sizeof (DISK_DETECTION_INFO);

					Irp->IoStatus.Information = fullOutputSize;
				}
				else
				{
					if (irpSp->Parameters.DeviceIoControl.OutputBufferLength >= sizeof (DISK_GEOMETRY_EX))
						Irp->IoStatus.Information = sizeof (DISK_GEOMETRY_EX);
					else
						Irp->IoStatus.Information = minOutputSize;
				}

				Irp->IoStatus.Status = STATUS_SUCCESS;
			}
		}
		break;

	case IOCTL_STORAGE_GET_MEDIA_TYPES_EX:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_STORAGE_GET_MEDIA_TYPES_EX)\n");
		if (ValidateIOBufferSize (Irp, sizeof (GET_MEDIA_TYPES), ValidateOutput))
		{
			PGET_MEDIA_TYPES outputBuffer = (PGET_MEDIA_TYPES)
			Irp->AssociatedIrp.SystemBuffer;
			PDEVICE_MEDIA_INFO mediaInfo = &outputBuffer->MediaInfo[0];

			outputBuffer->DeviceType = FILE_DEVICE_DISK;
			outputBuffer->MediaInfoCount = 1;

			if (Extension->bRemovable)
			{
				mediaInfo->DeviceSpecific.RemovableDiskInfo.NumberMediaSides = 1;
				if (Extension->bReadOnly)
					mediaInfo->DeviceSpecific.RemovableDiskInfo.MediaCharacteristics = (MEDIA_CURRENTLY_MOUNTED | MEDIA_READ_ONLY | MEDIA_WRITE_PROTECTED);
				else
					mediaInfo->DeviceSpecific.RemovableDiskInfo.MediaCharacteristics = (MEDIA_CURRENTLY_MOUNTED | MEDIA_READ_WRITE);
				mediaInfo->DeviceSpecific.RemovableDiskInfo.MediaType = (STORAGE_MEDIA_TYPE) RemovableMedia;
				mediaInfo->DeviceSpecific.RemovableDiskInfo.Cylinders.QuadPart = Extension->NumberOfCylinders;
				mediaInfo->DeviceSpecific.RemovableDiskInfo.TracksPerCylinder = Extension->TracksPerCylinder;
				mediaInfo->DeviceSpecific.RemovableDiskInfo.SectorsPerTrack = Extension->SectorsPerTrack;
				mediaInfo->DeviceSpecific.RemovableDiskInfo.BytesPerSector = Extension->BytesPerSector;
			}
			else
			{
				mediaInfo->DeviceSpecific.DiskInfo.NumberMediaSides = 1;
				if (Extension->bReadOnly)
					mediaInfo->DeviceSpecific.DiskInfo.MediaCharacteristics = (MEDIA_CURRENTLY_MOUNTED | MEDIA_READ_ONLY | MEDIA_WRITE_PROTECTED);
				else
					mediaInfo->DeviceSpecific.DiskInfo.MediaCharacteristics = (MEDIA_CURRENTLY_MOUNTED | MEDIA_READ_WRITE);
				mediaInfo->DeviceSpecific.DiskInfo.MediaType = (STORAGE_MEDIA_TYPE) FixedMedia;
				mediaInfo->DeviceSpecific.DiskInfo.Cylinders.QuadPart = Extension->NumberOfCylinders;
				mediaInfo->DeviceSpecific.DiskInfo.TracksPerCylinder = Extension->TracksPerCylinder;
				mediaInfo->DeviceSpecific.DiskInfo.SectorsPerTrack = Extension->SectorsPerTrack;
				mediaInfo->DeviceSpecific.DiskInfo.BytesPerSector = Extension->BytesPerSector;
			}
			Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = sizeof (GET_MEDIA_TYPES);
		}
		break;

	case IOCTL_STORAGE_QUERY_PROPERTY:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_STORAGE_QUERY_PROPERTY)\n");		
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		if (EnableExtendedIoctlSupport || Extension->TrimEnabled)
		{
			if (ValidateIOBufferSize (Irp, sizeof (STORAGE_PROPERTY_QUERY), ValidateInput))
			{
				PSTORAGE_PROPERTY_QUERY pStoragePropQuery = (PSTORAGE_PROPERTY_QUERY) Irp->AssociatedIrp.SystemBuffer;
				STORAGE_QUERY_TYPE type = pStoragePropQuery->QueryType;

				Dump ("IOCTL_STORAGE_QUERY_PROPERTY - PropertyId = %d, type = %d, InputBufferLength = %d, OutputBufferLength = %d\n", pStoragePropQuery->PropertyId, type, (int) irpSp->Parameters.DeviceIoControl.InputBufferLength, (int) irpSp->Parameters.DeviceIoControl.OutputBufferLength);

				if (Extension->bRawDevice &&
						(pStoragePropQuery->PropertyId == (STORAGE_PROPERTY_ID) StorageDeviceLBProvisioningProperty)
					)
				{
					IO_STATUS_BLOCK IoStatus;
					Dump ("ProcessVolumeDeviceControlIrp: sending IOCTL_STORAGE_QUERY_PROPERTY (%d) to device\n", (int) pStoragePropQuery->PropertyId);
					Irp->IoStatus.Status = ZwDeviceIoControlFile (
						Extension->hDeviceFile,
						NULL,
						NULL,
						NULL,
						&IoStatus,
						IOCTL_STORAGE_QUERY_PROPERTY,
						Irp->AssociatedIrp.SystemBuffer,
						irpSp->Parameters.DeviceIoControl.InputBufferLength,
						Irp->AssociatedIrp.SystemBuffer,
						irpSp->Parameters.DeviceIoControl.OutputBufferLength);
					Dump ("ProcessVolumeDeviceControlIrp: ZwDeviceIoControlFile returned 0x%.8X\n", (DWORD) Irp->IoStatus.Status);
					if (Irp->IoStatus.Status == STATUS_SUCCESS)
					{
						Irp->IoStatus.Status = IoStatus.Status;
						Irp->IoStatus.Information = IoStatus.Information;
					}
				}
				else if (	(pStoragePropQuery->PropertyId == StorageAccessAlignmentProperty)
					||	(pStoragePropQuery->PropertyId == StorageDeviceProperty)
					||	(pStoragePropQuery->PropertyId == StorageAdapterProperty)
					||	(pStoragePropQuery->PropertyId == StorageDeviceSeekPenaltyProperty)
					||	(pStoragePropQuery->PropertyId == StorageDeviceTrimProperty)
					)
				{
					if (type == PropertyExistsQuery)
					{
						Irp->IoStatus.Status = STATUS_SUCCESS;
						Irp->IoStatus.Information = 0;
					}
					else if (type == PropertyStandardQuery)
					{
						ULONG descriptorSize;
						switch (pStoragePropQuery->PropertyId)
						{
							case StorageDeviceProperty:
								{
									Dump ("IOCTL_STORAGE_QUERY_PROPERTY - StorageDeviceProperty\n");
									/* Add 0x00 for NULL terminating string used as ProductId, ProductRevision, SerialNumber, VendorId */
									descriptorSize = sizeof (STORAGE_DEVICE_DESCRIPTOR) + 1;
									if (ValidateIOBufferSize (Irp, descriptorSize, ValidateOutput))
									{
										PSTORAGE_DEVICE_DESCRIPTOR outputBuffer = (PSTORAGE_DEVICE_DESCRIPTOR) Irp->AssociatedIrp.SystemBuffer;

										outputBuffer->Version = sizeof(STORAGE_DEVICE_DESCRIPTOR);
										outputBuffer->Size = descriptorSize;
										outputBuffer->DeviceType = FILE_DEVICE_DISK;
										outputBuffer->RemovableMedia = Extension->bRemovable? TRUE : FALSE;
										outputBuffer->ProductIdOffset = sizeof (STORAGE_DEVICE_DESCRIPTOR);
										outputBuffer->SerialNumberOffset = sizeof (STORAGE_DEVICE_DESCRIPTOR);
										outputBuffer->ProductRevisionOffset = sizeof (STORAGE_DEVICE_DESCRIPTOR);
										outputBuffer->VendorIdOffset = sizeof (STORAGE_DEVICE_DESCRIPTOR);
										outputBuffer->BusType = BusTypeVirtual;
										Irp->IoStatus.Status = STATUS_SUCCESS;
										Irp->IoStatus.Information = descriptorSize;
									}
									else if (irpSp->Parameters.DeviceIoControl.OutputBufferLength == sizeof (STORAGE_DESCRIPTOR_HEADER))
									{
										PSTORAGE_DESCRIPTOR_HEADER outputBuffer = (PSTORAGE_DESCRIPTOR_HEADER) Irp->AssociatedIrp.SystemBuffer;
										outputBuffer->Version = sizeof(STORAGE_DEVICE_DESCRIPTOR);
										outputBuffer->Size = descriptorSize;
										Irp->IoStatus.Status = STATUS_SUCCESS;
										Irp->IoStatus.Information = sizeof (STORAGE_DESCRIPTOR_HEADER);
									}
								}
								break;
							case StorageAdapterProperty:
								{
									Dump ("IOCTL_STORAGE_QUERY_PROPERTY - StorageAdapterProperty\n");
									descriptorSize = sizeof (STORAGE_ADAPTER_DESCRIPTOR);
									if (ValidateIOBufferSize (Irp, descriptorSize, ValidateOutput))
									{
										PSTORAGE_ADAPTER_DESCRIPTOR outputBuffer = (PSTORAGE_ADAPTER_DESCRIPTOR) Irp->AssociatedIrp.SystemBuffer;

										outputBuffer->Version = sizeof(STORAGE_ADAPTER_DESCRIPTOR);
										outputBuffer->Size = descriptorSize;
										outputBuffer->MaximumTransferLength = Extension->HostMaximumTransferLength;
										outputBuffer->MaximumPhysicalPages = Extension->HostMaximumPhysicalPages;
										outputBuffer->AlignmentMask = Extension->HostAlignmentMask;
										outputBuffer->BusType = BusTypeVirtual;
										Irp->IoStatus.Status = STATUS_SUCCESS;
										Irp->IoStatus.Information = descriptorSize;
									}
									else if (irpSp->Parameters.DeviceIoControl.OutputBufferLength == sizeof (STORAGE_DESCRIPTOR_HEADER))
									{
										PSTORAGE_DESCRIPTOR_HEADER outputBuffer = (PSTORAGE_DESCRIPTOR_HEADER) Irp->AssociatedIrp.SystemBuffer;
										outputBuffer->Version = sizeof(STORAGE_ADAPTER_DESCRIPTOR);
										outputBuffer->Size = descriptorSize;
										Irp->IoStatus.Status = STATUS_SUCCESS;
										Irp->IoStatus.Information = sizeof (STORAGE_DESCRIPTOR_HEADER);
									}
								}
								break;
							case StorageAccessAlignmentProperty:
								{
									Dump ("IOCTL_STORAGE_QUERY_PROPERTY - StorageAccessAlignmentProperty\n");
									if (ValidateIOBufferSize (Irp, sizeof (STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR), ValidateOutput))
									{
										PSTORAGE_ACCESS_ALIGNMENT_DESCRIPTOR outputBuffer = (PSTORAGE_ACCESS_ALIGNMENT_DESCRIPTOR) Irp->AssociatedIrp.SystemBuffer;

										outputBuffer->Version = sizeof(STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR);
										outputBuffer->Size = sizeof(STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR);
										outputBuffer->BytesPerLogicalSector = Extension->BytesPerSector;
										outputBuffer->BytesPerPhysicalSector = Extension->HostBytesPerPhysicalSector;										
										Irp->IoStatus.Status = STATUS_SUCCESS;
										Irp->IoStatus.Information = sizeof (STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR);
									}
									else if (irpSp->Parameters.DeviceIoControl.OutputBufferLength == sizeof (STORAGE_DESCRIPTOR_HEADER))
									{
										PSTORAGE_DESCRIPTOR_HEADER outputBuffer = (PSTORAGE_DESCRIPTOR_HEADER) Irp->AssociatedIrp.SystemBuffer;
										outputBuffer->Version = sizeof(STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR);
										outputBuffer->Size = sizeof(STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR);
										Irp->IoStatus.Status = STATUS_SUCCESS;
										Irp->IoStatus.Information = sizeof (STORAGE_DESCRIPTOR_HEADER);
									}
								}
								break;
							case StorageDeviceSeekPenaltyProperty:
								{
									Dump ("IOCTL_STORAGE_QUERY_PROPERTY - StorageDeviceSeekPenaltyProperty\n");
									if (ValidateIOBufferSize (Irp, sizeof (DEVICE_SEEK_PENALTY_DESCRIPTOR), ValidateOutput))
									{
										PDEVICE_SEEK_PENALTY_DESCRIPTOR outputBuffer = (PDEVICE_SEEK_PENALTY_DESCRIPTOR) Irp->AssociatedIrp.SystemBuffer;
										Dump ("IOCTL_STORAGE_QUERY_PROPERTY - StorageDeviceSeekPenaltyProperty: set IncursSeekPenalty to %s\n", Extension->IncursSeekPenalty? "TRUE" : "FALSE");
										outputBuffer->Version = sizeof(DEVICE_SEEK_PENALTY_DESCRIPTOR);
										outputBuffer->Size = sizeof(DEVICE_SEEK_PENALTY_DESCRIPTOR);
										outputBuffer->IncursSeekPenalty = (BOOLEAN) Extension->IncursSeekPenalty;
										Irp->IoStatus.Status = STATUS_SUCCESS;
										Irp->IoStatus.Information = sizeof (DEVICE_SEEK_PENALTY_DESCRIPTOR);
									}
									else if (irpSp->Parameters.DeviceIoControl.OutputBufferLength == sizeof (STORAGE_DESCRIPTOR_HEADER))
									{
										PSTORAGE_DESCRIPTOR_HEADER outputBuffer = (PSTORAGE_DESCRIPTOR_HEADER) Irp->AssociatedIrp.SystemBuffer;
										outputBuffer->Version = sizeof(DEVICE_SEEK_PENALTY_DESCRIPTOR);
										outputBuffer->Size = sizeof(DEVICE_SEEK_PENALTY_DESCRIPTOR);
										Irp->IoStatus.Status = STATUS_SUCCESS;
										Irp->IoStatus.Information = sizeof (STORAGE_DESCRIPTOR_HEADER);
									}
								}
								break;
							case StorageDeviceTrimProperty:
								{
									Dump ("IOCTL_STORAGE_QUERY_PROPERTY - StorageDeviceTrimProperty\n");
									if (ValidateIOBufferSize (Irp, sizeof (DEVICE_TRIM_DESCRIPTOR), ValidateOutput))
									{
										PDEVICE_TRIM_DESCRIPTOR outputBuffer = (PDEVICE_TRIM_DESCRIPTOR) Irp->AssociatedIrp.SystemBuffer;
										Dump ("IOCTL_STORAGE_QUERY_PROPERTY - StorageDeviceTrimProperty: set TrimEnabled to %s\n", Extension->TrimEnabled? "TRUE" : "FALSE");
										outputBuffer->Version = sizeof(DEVICE_TRIM_DESCRIPTOR);
										outputBuffer->Size = sizeof(DEVICE_TRIM_DESCRIPTOR);
										outputBuffer->TrimEnabled = (BOOLEAN) Extension->TrimEnabled;
										Irp->IoStatus.Status = STATUS_SUCCESS;
										Irp->IoStatus.Information = sizeof (DEVICE_TRIM_DESCRIPTOR);
									}
									else if (irpSp->Parameters.DeviceIoControl.OutputBufferLength == sizeof (STORAGE_DESCRIPTOR_HEADER))
									{
										PSTORAGE_DESCRIPTOR_HEADER outputBuffer = (PSTORAGE_DESCRIPTOR_HEADER) Irp->AssociatedIrp.SystemBuffer;
										outputBuffer->Version = sizeof(DEVICE_TRIM_DESCRIPTOR);
										outputBuffer->Size = sizeof(DEVICE_TRIM_DESCRIPTOR);
										Irp->IoStatus.Status = STATUS_SUCCESS;
										Irp->IoStatus.Information = sizeof (STORAGE_DESCRIPTOR_HEADER);
									}
								}
								break;
						}
					}
				}
			}
		}

		break;

	case IOCTL_DISK_GET_PARTITION_INFO:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_DISK_GET_PARTITION_INFO)\n");
		if (ValidateIOBufferSize (Irp, sizeof (PARTITION_INFORMATION), ValidateOutput))
		{
			PPARTITION_INFORMATION outputBuffer = (PPARTITION_INFORMATION)
			Irp->AssociatedIrp.SystemBuffer;

			outputBuffer->PartitionType = Extension->PartitionType;
			outputBuffer->BootIndicator = FALSE;
			outputBuffer->RecognizedPartition = TRUE;
			outputBuffer->RewritePartition = FALSE;
			outputBuffer->StartingOffset.QuadPart = Extension->BytesPerSector;
			outputBuffer->PartitionLength.QuadPart= Extension->DiskLength;
			outputBuffer->PartitionNumber = 1;
			outputBuffer->HiddenSectors = 0;
			Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = sizeof (PARTITION_INFORMATION);
		}
		break;

	case IOCTL_DISK_GET_PARTITION_INFO_EX:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_DISK_GET_PARTITION_INFO_EX)\n");
		if (ValidateIOBufferSize (Irp, sizeof (PARTITION_INFORMATION_EX), ValidateOutput))
		{
			PPARTITION_INFORMATION_EX outputBuffer = (PPARTITION_INFORMATION_EX) Irp->AssociatedIrp.SystemBuffer;

			outputBuffer->PartitionStyle = PARTITION_STYLE_MBR;
			outputBuffer->RewritePartition = FALSE;
			outputBuffer->StartingOffset.QuadPart = Extension->BytesPerSector;
			outputBuffer->PartitionLength.QuadPart= Extension->DiskLength;
			outputBuffer->PartitionNumber = 1;
			outputBuffer->Mbr.PartitionType = Extension->PartitionType;
			outputBuffer->Mbr.BootIndicator = FALSE;
			outputBuffer->Mbr.RecognizedPartition = TRUE;
			outputBuffer->Mbr.HiddenSectors = 0;
			Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = sizeof (PARTITION_INFORMATION_EX);
		}
		break;

	case IOCTL_DISK_GET_DRIVE_LAYOUT:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_DISK_GET_DRIVE_LAYOUT)\n");
		if (ValidateIOBufferSize (Irp, sizeof (DRIVE_LAYOUT_INFORMATION), ValidateOutput))
		{
			PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation (Irp);
			BOOL bFullBuffer = (irpSp->Parameters.DeviceIoControl.OutputBufferLength >= (sizeof (DRIVE_LAYOUT_INFORMATION) + 3*sizeof(PARTITION_INFORMATION)))? TRUE : FALSE;
			PDRIVE_LAYOUT_INFORMATION outputBuffer = (PDRIVE_LAYOUT_INFORMATION)
			Irp->AssociatedIrp.SystemBuffer;

			outputBuffer->PartitionCount = bFullBuffer? 4 : 1;
			outputBuffer->Signature = GetCrc32((unsigned char*) &(Extension->UniqueVolumeId), 4);

			outputBuffer->PartitionEntry->PartitionType = Extension->PartitionType;
			outputBuffer->PartitionEntry->BootIndicator = FALSE;
			outputBuffer->PartitionEntry->RecognizedPartition = TRUE;
			outputBuffer->PartitionEntry->RewritePartition = FALSE;
			outputBuffer->PartitionEntry->StartingOffset.QuadPart = Extension->BytesPerSector;
			outputBuffer->PartitionEntry->PartitionLength.QuadPart = Extension->DiskLength;
			outputBuffer->PartitionEntry->PartitionNumber = 1;
			outputBuffer->PartitionEntry->HiddenSectors = 0;			

			Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = sizeof (DRIVE_LAYOUT_INFORMATION);
			if (bFullBuffer)
			{
				Irp->IoStatus.Information += 3*sizeof(PARTITION_INFORMATION);
				memset (((BYTE*) Irp->AssociatedIrp.SystemBuffer) + sizeof (DRIVE_LAYOUT_INFORMATION), 0, 3*sizeof(PARTITION_INFORMATION));
			}				
		}
		break;

	case IOCTL_DISK_GET_DRIVE_LAYOUT_EX:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_DISK_GET_DRIVE_LAYOUT_EX)\n");
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		if (EnableExtendedIoctlSupport)
		{
			if (ValidateIOBufferSize (Irp, sizeof (DRIVE_LAYOUT_INFORMATION_EX), ValidateOutput))
			{
				PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation (Irp);
				BOOL bFullBuffer = (irpSp->Parameters.DeviceIoControl.OutputBufferLength >= (sizeof (DRIVE_LAYOUT_INFORMATION_EX) + 3*sizeof(PARTITION_INFORMATION_EX)))? TRUE : FALSE;
				PDRIVE_LAYOUT_INFORMATION_EX outputBuffer = (PDRIVE_LAYOUT_INFORMATION_EX)
				Irp->AssociatedIrp.SystemBuffer;

				outputBuffer->PartitionCount = bFullBuffer? 4 : 1;
				outputBuffer->PartitionStyle = PARTITION_STYLE_MBR;
				outputBuffer->Mbr.Signature = GetCrc32((unsigned char*) &(Extension->UniqueVolumeId), 4);

				outputBuffer->PartitionEntry->PartitionStyle = PARTITION_STYLE_MBR;
				outputBuffer->PartitionEntry->Mbr.BootIndicator = FALSE;
				outputBuffer->PartitionEntry->Mbr.RecognizedPartition = TRUE;
				outputBuffer->PartitionEntry->RewritePartition = FALSE;
				outputBuffer->PartitionEntry->StartingOffset.QuadPart = Extension->BytesPerSector;
				outputBuffer->PartitionEntry->PartitionLength.QuadPart = Extension->DiskLength;
				outputBuffer->PartitionEntry->PartitionNumber = 1;
				outputBuffer->PartitionEntry->Mbr.HiddenSectors = 0;
				outputBuffer->PartitionEntry->Mbr.PartitionType = Extension->PartitionType;

				Irp->IoStatus.Status = STATUS_SUCCESS;
				Irp->IoStatus.Information = sizeof (DRIVE_LAYOUT_INFORMATION_EX);
				if (bFullBuffer)
				{
					Irp->IoStatus.Information += 3*sizeof(PARTITION_INFORMATION_EX);
				}
			}
		}
		break;

	case IOCTL_DISK_GET_LENGTH_INFO:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_DISK_GET_LENGTH_INFO)\n");
		if (!ValidateIOBufferSize (Irp, sizeof (GET_LENGTH_INFORMATION), ValidateOutput))
		{
			Irp->IoStatus.Status = STATUS_BUFFER_OVERFLOW;
			Irp->IoStatus.Information = sizeof (GET_LENGTH_INFORMATION);
		}
		else
		{
			PGET_LENGTH_INFORMATION outputBuffer = (PGET_LENGTH_INFORMATION) Irp->AssociatedIrp.SystemBuffer;

			outputBuffer->Length.QuadPart = Extension->DiskLength;
			Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = sizeof (GET_LENGTH_INFORMATION);
		}
		break;

	case IOCTL_DISK_VERIFY:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_DISK_VERIFY)\n");
		if (ValidateIOBufferSize (Irp, sizeof (VERIFY_INFORMATION), ValidateInput))
		{
			HRESULT hResult;
			ULONGLONG ullStartingOffset, ullNewOffset, ullEndOffset;
			PVERIFY_INFORMATION pVerifyInformation;
			pVerifyInformation = (PVERIFY_INFORMATION) Irp->AssociatedIrp.SystemBuffer;

			ullStartingOffset = (ULONGLONG) pVerifyInformation->StartingOffset.QuadPart;
			hResult = ULongLongAdd(ullStartingOffset,
				(ULONGLONG) Extension->cryptoInfo->hiddenVolume ? Extension->cryptoInfo->hiddenVolumeOffset : Extension->cryptoInfo->volDataAreaOffset,
				&ullNewOffset);
			if (hResult != S_OK)
				Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
			else if (S_OK != ULongLongAdd(ullStartingOffset, (ULONGLONG) pVerifyInformation->Length, &ullEndOffset))
				Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
			else if (ullEndOffset > (ULONGLONG) Extension->DiskLength)
				Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
			else
			{
				IO_STATUS_BLOCK ioStatus;
				PVOID buffer = TCalloc (max (pVerifyInformation->Length, PAGE_SIZE));

				if (!buffer)
				{
					Irp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
				}
				else
				{
					LARGE_INTEGER offset = pVerifyInformation->StartingOffset;
					offset.QuadPart = ullNewOffset;

					Irp->IoStatus.Status = ZwReadFile (Extension->hDeviceFile, NULL, NULL, NULL, &ioStatus, buffer, pVerifyInformation->Length, &offset, NULL);
					TCfree (buffer);

					if (NT_SUCCESS (Irp->IoStatus.Status) && ioStatus.Information != pVerifyInformation->Length)
						Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
				}
			}

			Irp->IoStatus.Information = 0;
		}
		break;

	case IOCTL_DISK_CHECK_VERIFY:
	case IOCTL_STORAGE_CHECK_VERIFY:
	case IOCTL_STORAGE_CHECK_VERIFY2:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_STORAGE_CHECK_VERIFY)\n");
		{
			Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = 0;

			if (irpSp->Parameters.DeviceIoControl.OutputBufferLength >= sizeof (ULONG))
			{
				*((ULONG *) Irp->AssociatedIrp.SystemBuffer) = 0;
				Irp->IoStatus.Information = sizeof (ULONG);
			}
		}
		break;

	case IOCTL_DISK_IS_WRITABLE:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_DISK_IS_WRITABLE)\n");
		{
			if (Extension->bReadOnly)
				Irp->IoStatus.Status = STATUS_MEDIA_WRITE_PROTECTED;
			else
				Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = 0;

		}
		break;

	case IOCTL_VOLUME_ONLINE:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_VOLUME_ONLINE)\n");
		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = 0;
		break;

	case IOCTL_VOLUME_POST_ONLINE:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_VOLUME_POST_ONLINE)\n");
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		if (EnableExtendedIoctlSupport)
		{
			Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = 0;
		}
		break;

	case IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS)\n");
		if (!(OsMajorVersion == 6 && OsMinorVersion == 0) 
			&& !(IsOSAtLeast (WIN_8_1) && AllowWindowsDefrag && Extension->bRawDevice)
			)
		{
			Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
			Irp->IoStatus.Information = 0;
		}
		else if (ValidateIOBufferSize (Irp, sizeof (VOLUME_DISK_EXTENTS), ValidateOutput))
		{
			VOLUME_DISK_EXTENTS *extents = (VOLUME_DISK_EXTENTS *) Irp->AssociatedIrp.SystemBuffer;
			

			if (IsOSAtLeast (WIN_8_1))
			{
				extents->NumberOfDiskExtents = 1;
				extents->Extents[0].DiskNumber = Extension->DeviceNumber;
				extents->Extents[0].StartingOffset.QuadPart = Extension->BytesPerSector;
				extents->Extents[0].ExtentLength.QuadPart = Extension->DiskLength;
			}
			else
			{
				memset (extents, 0, sizeof (*extents));
				extents->NumberOfDiskExtents = 0;
			}

			Irp->IoStatus.Status = STATUS_SUCCESS;
			Irp->IoStatus.Information = sizeof (*extents);
		}
		break;

	case IOCTL_STORAGE_READ_CAPACITY:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_STORAGE_READ_CAPACITY)\n");
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		if (EnableExtendedIoctlSupport)
		{
			if (ValidateIOBufferSize (Irp, sizeof (STORAGE_READ_CAPACITY), ValidateOutput))
			{
				STORAGE_READ_CAPACITY *capacity = (STORAGE_READ_CAPACITY *) Irp->AssociatedIrp.SystemBuffer;

				capacity->Version = sizeof (STORAGE_READ_CAPACITY);
				capacity->Size = sizeof (STORAGE_READ_CAPACITY);
				capacity->BlockLength = Extension->BytesPerSector;
				capacity->NumberOfBlocks.QuadPart = (Extension->DiskLength / Extension->BytesPerSector) + 1;
				capacity->DiskLength.QuadPart = Extension->DiskLength + Extension->BytesPerSector;

				Irp->IoStatus.Status = STATUS_SUCCESS;
				Irp->IoStatus.Information = sizeof (STORAGE_READ_CAPACITY);
			}
		}
		break;

	/*case IOCTL_STORAGE_GET_DEVICE_NUMBER:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_STORAGE_GET_DEVICE_NUMBER)\n");
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		if (EnableExtendedIoctlSupport)
		{
			if (ValidateIOBufferSize (Irp, sizeof (STORAGE_DEVICE_NUMBER), ValidateOutput))
			{
				STORAGE_DEVICE_NUMBER *storage = (STORAGE_DEVICE_NUMBER *) Irp->AssociatedIrp.SystemBuffer;

				storage->DeviceType = FILE_DEVICE_DISK;
				storage->DeviceNumber = (ULONG) -1;
				storage->PartitionNumber = 1;

				Irp->IoStatus.Status = STATUS_SUCCESS;
				Irp->IoStatus.Information = sizeof (STORAGE_DEVICE_NUMBER);
			}
		}
		break;*/

	case IOCTL_STORAGE_GET_HOTPLUG_INFO:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_STORAGE_GET_HOTPLUG_INFO)\n");
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		if (EnableExtendedIoctlSupport)
		{
			if (ValidateIOBufferSize (Irp, sizeof (STORAGE_HOTPLUG_INFO), ValidateOutput))
			{
				STORAGE_HOTPLUG_INFO *info = (STORAGE_HOTPLUG_INFO *) Irp->AssociatedIrp.SystemBuffer;

				info->Size = sizeof (STORAGE_HOTPLUG_INFO);
				info->MediaRemovable = Extension->bRemovable? TRUE : FALSE;
				info->MediaHotplug = FALSE;
				info->DeviceHotplug = FALSE;
				info->WriteCacheEnableOverride = FALSE;

				Irp->IoStatus.Status = STATUS_SUCCESS;
				Irp->IoStatus.Information = sizeof (STORAGE_HOTPLUG_INFO);
			}
		}
		break;

	case IOCTL_VOLUME_IS_DYNAMIC:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_VOLUME_IS_DYNAMIC)\n");
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		if (EnableExtendedIoctlSupport)
		{
			if (ValidateIOBufferSize (Irp, sizeof (BOOLEAN), ValidateOutput))
			{
				BOOLEAN *pbDynamic = (BOOLEAN*) Irp->AssociatedIrp.SystemBuffer;

				*pbDynamic = FALSE;

				Irp->IoStatus.Status = STATUS_SUCCESS;
				Irp->IoStatus.Information = sizeof (BOOLEAN);
			}
		}
		break;

	case IOCTL_DISK_IS_CLUSTERED:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_DISK_IS_CLUSTERED)\n");
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		if (EnableExtendedIoctlSupport)
		{
			if (ValidateIOBufferSize (Irp, sizeof (BOOLEAN), ValidateOutput))
			{
				BOOLEAN *pbIsClustered = (BOOLEAN*) Irp->AssociatedIrp.SystemBuffer;

				*pbIsClustered = FALSE;

				Irp->IoStatus.Status = STATUS_SUCCESS;
				Irp->IoStatus.Information = sizeof (BOOLEAN);
			}
		}
		break;

	case IOCTL_VOLUME_GET_GPT_ATTRIBUTES:
		Dump ("ProcessVolumeDeviceControlIrp (IOCTL_VOLUME_GET_GPT_ATTRIBUTES)\n");
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		if (EnableExtendedIoctlSupport)
		{
			if (ValidateIOBufferSize (Irp, sizeof (VOLUME_GET_GPT_ATTRIBUTES_INFORMATION), ValidateOutput))
			{
				VOLUME_GET_GPT_ATTRIBUTES_INFORMATION *pGptAttr = (VOLUME_GET_GPT_ATTRIBUTES_INFORMATION*) Irp->AssociatedIrp.SystemBuffer;

				pGptAttr->GptAttributes = 0; // we are MBR not GPT

				Irp->IoStatus.Status = STATUS_SUCCESS;
				Irp->IoStatus.Information = sizeof (VOLUME_GET_GPT_ATTRIBUTES_INFORMATION);
			}
		}
		break;

	case IOCTL_UNKNOWN_WINDOWS10_EFS_ACCESS:
		Dump ("ProcessVolumeDeviceControlIrp (unknown IOCTL 0x%.8X, OutputBufferLength = %d). Returning fake success\n", irpSp->Parameters.DeviceIoControl.IoControlCode, (int) irpSp->Parameters.DeviceIoControl.OutputBufferLength);
		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = 0;

		break;

	case IOCTL_DISK_UPDATE_PROPERTIES:
		Dump ("ProcessVolumeDeviceControlIrp: returning STATUS_SUCCESS for IOCTL_DISK_UPDATE_PROPERTIES\n");
		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = 0;

		break;

	case IOCTL_DISK_MEDIA_REMOVAL:
	case IOCTL_STORAGE_MEDIA_REMOVAL:
		Dump ("ProcessVolumeDeviceControlIrp: returning STATUS_SUCCESS for %ls\n", TCTranslateCode (irpSp->Parameters.DeviceIoControl.IoControlCode));
		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = 0;

		break;

	case IOCTL_DISK_GET_CLUSTER_INFO:
		Dump ("ProcessVolumeDeviceControlIrp: returning STATUS_NOT_SUPPORTED for %ls\n", TCTranslateCode (irpSp->Parameters.DeviceIoControl.IoControlCode));
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		if (EnableExtendedIoctlSupport)
		{
			Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
			Irp->IoStatus.Information = 0;
		}
		break;

	case IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES:
		Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES\n");
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		if (Extension->bRawDevice && Extension->TrimEnabled)
		{
			if (ValidateIOBufferSize (Irp, sizeof (DEVICE_MANAGE_DATA_SET_ATTRIBUTES), ValidateInput))
			{
				PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation (Irp);
				DWORD inputLength = irpSp->Parameters.DeviceIoControl.InputBufferLength;
				PDEVICE_MANAGE_DATA_SET_ATTRIBUTES pInputAttrs = (PDEVICE_MANAGE_DATA_SET_ATTRIBUTES) Irp->AssociatedIrp.SystemBuffer;
				DEVICE_DATA_MANAGEMENT_SET_ACTION action = pInputAttrs->Action;
				BOOL bEntireSet = pInputAttrs->Flags & DEVICE_DSM_FLAG_ENTIRE_DATA_SET_RANGE? TRUE : FALSE;
				ULONGLONG minSizedataSet = (ULONGLONG) pInputAttrs->DataSetRangesOffset + (ULONGLONG) pInputAttrs->DataSetRangesLength;
				ULONGLONG minSizeParameter = (ULONGLONG) pInputAttrs->ParameterBlockOffset + (ULONGLONG) pInputAttrs->ParameterBlockLength;
				ULONGLONG minSizeGeneric = sizeof(DEVICE_MANAGE_DATA_SET_ATTRIBUTES) + (ULONGLONG) pInputAttrs->ParameterBlockLength + (ULONGLONG) pInputAttrs->DataSetRangesLength;
				PDEVICE_MANAGE_DATA_SET_ATTRIBUTES pNewSetAttrs = NULL;
				ULONG ulNewInputLength = 0;
				BOOL bForwardIoctl = FALSE;

				if (inputLength >= minSizeGeneric && inputLength >= minSizedataSet && inputLength >= minSizeParameter)
				{
					if (bEntireSet)
					{
						if (minSizedataSet)
						{
							Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - DEVICE_DSM_FLAG_ENTIRE_DATA_SET_RANGE set but data set range specified=> Error.\n");
							Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
							Irp->IoStatus.Information = 0;
						}
						else
						{
							DWORD dwDataSetOffset = ALIGN_VALUE (inputLength, sizeof(DEVICE_DATA_SET_RANGE));
							DWORD dwDataSetLength = sizeof(DEVICE_DATA_SET_RANGE);

							Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - DEVICE_DSM_FLAG_ENTIRE_DATA_SET_RANGE set. Setting data range to all volume.\n");

							ulNewInputLength = dwDataSetOffset + dwDataSetLength;
							pNewSetAttrs = (PDEVICE_MANAGE_DATA_SET_ATTRIBUTES) TCalloc (ulNewInputLength);
							if (pNewSetAttrs)
							{
								PDEVICE_DATA_SET_RANGE pRange = (PDEVICE_DATA_SET_RANGE) (((unsigned char*) pNewSetAttrs) + dwDataSetOffset);

								memcpy (pNewSetAttrs, pInputAttrs, inputLength);

								pRange->StartingOffset = (ULONGLONG) Extension->cryptoInfo->hiddenVolume ? Extension->cryptoInfo->hiddenVolumeOffset : Extension->cryptoInfo->volDataAreaOffset;
								pRange->LengthInBytes = Extension->DiskLength;

								pNewSetAttrs->Size = sizeof(DEVICE_MANAGE_DATA_SET_ATTRIBUTES);
								pNewSetAttrs->Action = action;
								pNewSetAttrs->Flags = pInputAttrs->Flags & (~DEVICE_DSM_FLAG_ENTIRE_DATA_SET_RANGE);
								pNewSetAttrs->ParameterBlockOffset = pInputAttrs->ParameterBlockOffset;
								pNewSetAttrs->ParameterBlockLength = pInputAttrs->ParameterBlockLength;
								pNewSetAttrs->DataSetRangesOffset = dwDataSetOffset;
								pNewSetAttrs->DataSetRangesLength = dwDataSetLength;

								bForwardIoctl = TRUE;
							}
							else
							{
								Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - Failed to allocate memory.\n");
								Irp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
								Irp->IoStatus.Information = 0;
							}
						}
					}
					else
					{						
						Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - creating new data set range from input range.\n");
						ulNewInputLength = inputLength;
						pNewSetAttrs = (PDEVICE_MANAGE_DATA_SET_ATTRIBUTES) TCalloc (inputLength);
						if (pNewSetAttrs)
						{
							PDEVICE_DATA_SET_RANGE pNewRanges = (PDEVICE_DATA_SET_RANGE) (((unsigned char*) pNewSetAttrs) + pInputAttrs->DataSetRangesOffset);
							PDEVICE_DATA_SET_RANGE pInputRanges = (PDEVICE_DATA_SET_RANGE) (((unsigned char*) pInputAttrs) + pInputAttrs->DataSetRangesOffset);
							DWORD dwInputRangesCount = 0, dwNewRangesCount = 0, i;
							ULONGLONG ullStartingOffset, ullNewOffset, ullEndOffset;
							HRESULT hResult;

							memcpy (pNewSetAttrs, pInputAttrs, inputLength);

							dwInputRangesCount = pInputAttrs->DataSetRangesLength / sizeof(DEVICE_DATA_SET_RANGE);

							for (i = 0; i < dwInputRangesCount; i++)
							{
								ullStartingOffset = (ULONGLONG) pInputRanges[i].StartingOffset;
								hResult = ULongLongAdd(ullStartingOffset,
									(ULONGLONG) Extension->cryptoInfo->hiddenVolume ? Extension->cryptoInfo->hiddenVolumeOffset : Extension->cryptoInfo->volDataAreaOffset,
									&ullNewOffset);
								if (hResult != S_OK)
									continue;
								else if (S_OK != ULongLongAdd(ullStartingOffset, (ULONGLONG) pInputRanges[i].LengthInBytes, &ullEndOffset))
									continue;
								else if (ullEndOffset > (ULONGLONG) Extension->DiskLength)
									continue;
								else if (ullNewOffset > 0)
								{
									pNewRanges[dwNewRangesCount].StartingOffset = (LONGLONG) ullNewOffset;
									pNewRanges[dwNewRangesCount].LengthInBytes = pInputRanges[i].LengthInBytes;

									dwNewRangesCount++;
								}
							}

							Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - %d valid range processed from %d range in input.\n", (int) dwNewRangesCount, (int) dwInputRangesCount);

							pNewSetAttrs->DataSetRangesLength = dwNewRangesCount * sizeof (DEVICE_DATA_SET_RANGE);

							bForwardIoctl = TRUE;
						}
						else
						{
							Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - Failed to allocate memory.\n");
							Irp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
							Irp->IoStatus.Information = 0;
						}
					}
				}
				else
				{
					Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - buffer containing DEVICE_MANAGE_DATA_SET_ATTRIBUTES has invalid length.\n");
					Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
					Irp->IoStatus.Information = 0;
				}


				if (bForwardIoctl)
				{
					if (action == DeviceDsmAction_Trim)
					{
						Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - DeviceDsmAction_Trim.\n");

						if (Extension->cryptoInfo->hiddenVolume || !AllowTrimCommand)
						{
							Dump ("ProcessVolumeDeviceControlIrp: TRIM command filtered\n");
							Irp->IoStatus.Status = STATUS_SUCCESS;
							Irp->IoStatus.Information = 0;
						}
						else
						{
							IO_STATUS_BLOCK IoStatus;
							Dump ("ProcessVolumeDeviceControlIrp: sending TRIM to device\n");
							Irp->IoStatus.Status = ZwDeviceIoControlFile (
								Extension->hDeviceFile,
								NULL,
								NULL,
								NULL,
								&IoStatus,
								IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES,
								(PVOID) pNewSetAttrs,
								ulNewInputLength,
								NULL,
								0);
							Dump ("ProcessVolumeDeviceControlIrp: ZwDeviceIoControlFile returned 0x%.8X\n", (DWORD) Irp->IoStatus.Status);
							if (Irp->IoStatus.Status == STATUS_SUCCESS)
							{
								Irp->IoStatus.Status = IoStatus.Status;
								Irp->IoStatus.Information = IoStatus.Information;
							}
							else
								Irp->IoStatus.Information = 0;
						}						
					}
					else
					{
						switch (action)
						{
							case DeviceDsmAction_Notification: Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - DeviceDsmAction_Notification\n"); break;
							case DeviceDsmAction_OffloadRead: Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - DeviceDsmAction_OffloadRead\n"); break;
							case DeviceDsmAction_OffloadWrite: Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - DeviceDsmAction_OffloadWrite\n"); break;
							case DeviceDsmAction_Allocation: Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - DeviceDsmAction_Allocation\n"); break;
							case DeviceDsmAction_Scrub: Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - DeviceDsmAction_Scrub\n"); break;
							case DeviceDsmAction_DrtQuery: Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - DeviceDsmAction_DrtQuery\n"); break;
							case DeviceDsmAction_DrtClear: Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - DeviceDsmAction_DrtClear\n"); break;
							case DeviceDsmAction_DrtDisable: Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - DeviceDsmAction_DrtDisable\n"); break;
							default: Dump ("ProcessVolumeDeviceControlIrp: IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES - unknown action %d\n", (int) action); break;
						}
					
					}
				}

				if (pNewSetAttrs)
					TCfree (pNewSetAttrs);
			}
		}
#if defined (DEBUG) || defined (DEBUG_TRACE)
		else
			Dump ("ProcessVolumeDeviceControlIrp: returning STATUS_INVALID_DEVICE_REQUEST for IOCTL_STORAGE_MANAGE_DATA_SET_ATTRIBUTES\n");
#endif
		break;
	
	case IOCTL_STORAGE_CHECK_PRIORITY_HINT_SUPPORT:
	case IOCTL_VOLUME_QUERY_ALLOCATION_HINT:
	case FT_BALANCED_READ_MODE:
	case IOCTL_STORAGE_GET_DEVICE_NUMBER:
	case IOCTL_MOUNTDEV_LINK_CREATED:
		Dump ("ProcessVolumeDeviceControlIrp: returning STATUS_INVALID_DEVICE_REQUEST for %ls\n", TCTranslateCode (irpSp->Parameters.DeviceIoControl.IoControlCode));
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;		
		break;
	default:
		Dump ("ProcessVolumeDeviceControlIrp (unknown code 0x%.8X)\n", irpSp->Parameters.DeviceIoControl.IoControlCode);
		return TCCompleteIrp (Irp, STATUS_INVALID_DEVICE_REQUEST, 0);
	}

#if defined(DEBUG) || defined (DEBG_TRACE)
	if (!NT_SUCCESS (Irp->IoStatus.Status))
	{
		Dump ("IOCTL error 0x%08x (0x%x %d)\n",
			Irp->IoStatus.Status,
			(int) (irpSp->Parameters.DeviceIoControl.IoControlCode >> 16),
			(int) ((irpSp->Parameters.DeviceIoControl.IoControlCode & 0x1FFF) >> 2));
	}
#endif

	return TCCompleteDiskIrp (Irp, Irp->IoStatus.Status, Irp->IoStatus.Information);
}

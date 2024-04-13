PP_Resource PPB_ImageData_Impl::CreatePlatform(PP_Instance instance,
                                               PP_ImageDataFormat format,
                                               const PP_Size& size,
                                               PP_Bool init_to_zero) {
  scoped_refptr<PPB_ImageData_Impl>
      data(new PPB_ImageData_Impl(instance, PLATFORM));
  if (!data->Init(format, size.width, size.height, !!init_to_zero))
    return 0;
  return data->GetReference();
}

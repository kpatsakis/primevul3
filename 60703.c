void Type_LUT8_Free(struct _cms_typehandler_struct* self, void* Ptr)
{
    cmsPipelineFree((cmsPipeline*) Ptr);
    return;

    cmsUNUSED_PARAMETER(self);
}

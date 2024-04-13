void* AllocChunk(cmsIT8* it8, cmsUInt32Number size)
{
    cmsUInt32Number Free = it8 ->Allocator.BlockSize - it8 ->Allocator.Used;
    cmsUInt8Number* ptr;

    size = _cmsALIGNMEM(size);

    if (size > Free) {

        if (it8 -> Allocator.BlockSize == 0)

                it8 -> Allocator.BlockSize = 20*1024;
        else
                it8 ->Allocator.BlockSize *= 2;

        if (it8 ->Allocator.BlockSize < size)
                it8 ->Allocator.BlockSize = size;

        it8 ->Allocator.Used = 0;
        it8 ->Allocator.Block = (cmsUInt8Number*)  AllocBigBlock(it8, it8 ->Allocator.BlockSize);
    }

    ptr = it8 ->Allocator.Block + it8 ->Allocator.Used;
    it8 ->Allocator.Used += size;

    return (void*) ptr;

}

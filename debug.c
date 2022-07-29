#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

const char* dump_memory(const char* filename, unsigned char* start_address, int length) {
    //dump_memory("D:\\Source\\dev\\mpeg\\iso 13818-2\\output\\frame0.rgb", bgr_frame_buffer, 1997568)
    //dump_memory("D:\\Source\\dev\\mpeg\\iso 13818-2\\output\\frame0.bin", newframe[0], 622080)
    //dump_memory("D:\\Source\\dev\\mpeg\\iso 13818-2\\output\\dct732.bin", (unsigned char*)QF[2], 128)
//dump_memory("D:\\Source\\dev\\mpeg\\iso 13818-2\\output\\frame13.full", (unsigned char*)auxframe[0], 622080)

    static char buffer[80];
    FILE* fp;
    if (fopen_s(&fp, filename, "wb"))
    {
        strerror_s(buffer, 80, errno);
        return buffer;
    }
    for (int count = 0; count < length; count++)
        if (fputc(*start_address++, fp) == EOF)
        {
            strerror_s(buffer, 80, errno);
            return buffer;
        }
    fclose(fp);
    return "succeess";
}


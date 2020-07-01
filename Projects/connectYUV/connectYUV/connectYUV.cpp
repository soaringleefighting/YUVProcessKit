#include "stdio.h"
#include "stdlib.h"

int main(int argc, char** argv)
{
	unsigned char* y;
	FILE* fp_yuv1, *fp_yuv2, *fp_yuv3;
	int width, height, len1, len2, picturesize;
	int i;

	if (argc < 8)
	{
		printf("Usage: connectYUV.exe src2file src2file dstfile width height len1 len2\n\n");
		system("pause");
		return -1;
	}


	fp_yuv1 = fopen(argv[1], "rb");
	if (NULL == fp_yuv1)
	{
		printf("ERROR: open %s fail!\n", argv[1]);
		return -1;
	}
	fp_yuv2 = fopen(argv[2], "rb");
	if (NULL == fp_yuv2)
	{
		printf("ERROR: open %s fail!\n", argv[2]);
		return -1;
	}
	fp_yuv3 = fopen(argv[3], "wb");
	if (NULL == fp_yuv3)
	{
		printf("ERROR: open %s fail!\n", argv[3]);
		return -1;
	}

	width = atoi(argv[4]);
	height = atoi(argv[5]);
	len1 = atoi(argv[6]);
	len2 = atoi(argv[7]);

	picturesize = width *height * 3 / 2; //++YUV420
	y = (unsigned char*)malloc(picturesize*sizeof(unsigned char*));
	if (NULL == y)
	{
		printf("ERROR: malloc y fail!\n");
	}

	for (i = 0; i < len1; i++)
	{
		if (fread(y, 1, picturesize, fp_yuv1) == picturesize)
		{
			fwrite(y, 1, picturesize, fp_yuv3);
		}
	}
	for (i = 0; i < len2; i++)
	{
		if (fread(y, 1, picturesize, fp_yuv2) == picturesize)
		{
			fwrite(y, 1, picturesize, fp_yuv3);
		}
	}

	printf("connect yuv %s and %s ==> %s successfully, total frames: %d\n", argv[1], argv[2], argv[3], len1+len2);

	free(y);
	y = NULL;
	fclose(fp_yuv1);
	fclose(fp_yuv2);
	fclose(fp_yuv3);

	return 0;
}
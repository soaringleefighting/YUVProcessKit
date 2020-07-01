/*****************************************************
//Function: split a yuv file to two sub yuv file, fps 
// increase twice.
//Date: 2018-5-30
//Author: SoaringLee
//Modified:
*****************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "string"

int main(int argc, char** argv)
{
	unsigned char* y;
	FILE* fp_input, *fp_out1, *fp_out2;
	int width, height, picturesize, framenum = 0;


	if (argc < 5)
	{
		printf("Usage: splitYUV.exe  srcyuv dstyuv1 dstyuv2 width height\n\n");
		system("pause");
		return -1;
	}


	width = atoi(argv[4]);
	height = atoi(argv[5]);

	fp_input = fopen(argv[1], "rb");
	if (NULL == fp_input)
	{
		printf("open %s fail!\n", argv[1]);
		return -1;
	}

	fp_out1 = fopen(argv[2], "wb");
	if (NULL == fp_out1)
	{
		printf("open %s fail!\n", argv[2]);
		return -1;
	}

	fp_out2 = fopen(argv[3], "wb");
	if (NULL == fp_out2)
	{
		printf("open %s fail!\n", argv[3]);
		return -1;
	}

	picturesize = width*height * 3 / 2;//++YUV420

	y = (unsigned char*)malloc(picturesize*sizeof(unsigned char*));

	while (fread(y, 1, picturesize, fp_input) == picturesize)
	{
		if (framenum % 2 == 0)
		{
			fwrite(y, 1, picturesize, fp_out1);
		}
		else
		{
			fwrite(y, 1, picturesize, fp_out2);
		}

		printf("%dth frame ok!!\n", framenum);

		framenum++;
	}

	printf("[SplitYUV] split %s to %s and %s successfully!!!,total frames: %d\n", argv[1], argv[2], argv[3], framenum);

	free(y);
	y = NULL;
	fclose(fp_input);
	fclose(fp_out1);
	fclose(fp_out2);

	return 0;
}
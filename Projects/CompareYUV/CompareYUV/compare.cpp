#include<stdio.h>
#include <stdlib.h>
#include <string>

int main(int argc, char **argv)
{
	FILE *fin_1 = NULL;
	FILE *fin_2 = NULL;
	FILE *fout = NULL;

	int srcwidth = 0, srcheight = 0;
	int framesize = 0, in1_size = 0, in2_size = 0;
	int srcframenum = 0;
	unsigned char *buffer1 = NULL;
	unsigned char *buffer2 = NULL;

	if (argc!=5)
	{
		printf("Usage: CompareYUV.exe yuv0 yuv1 width height\n");
		system("pause");
		return -1;
	}
	fin_1 = fopen(argv[1],"rb");
	if (!fin_1)
	{
		printf("Open input yuv0 failed!\n");
	}
	fin_2 = fopen(argv[2], "rb");
	if (!fin_2)
	{
		printf("Open input yuv1 failed!\n");
	}
	fout = fopen("compare_result.txt", "a+");
	if (!fout)
	{
		printf("Open compare_result.txt failed!\n");
	}

	srcwidth = atoi(argv[3]);//width
	srcheight = atoi(argv[4]);//height

	framesize = srcheight*srcwidth * 3 / 2;//++YUV420采样格式


	buffer1 = (unsigned char *)malloc(framesize*sizeof(unsigned char*));
	buffer2 = (unsigned char *)malloc(framesize*sizeof(unsigned char*));
	if ((buffer1 == NULL) ||(buffer2 == NULL))
	{
		printf("malloc buffer1 or buffer2 failed!\n");
	}

	fprintf(fout, "%s VS %s!    ", argv[1], argv[2]);
	while ((fread(buffer1, 1, framesize, fin_1) == framesize)&&(fread(buffer2, 1, framesize, fin_2) == framesize))
	{
		int i = 0;
		for (i = 0; i < framesize; i++)
		{
			if (buffer1[i] != buffer2[i])
			{
				fprintf(fout, "framenum:%d  ", srcframenum);
				fprintf(fout, "DISMATCH!\n");
				return -1;
			}
		}
		srcframenum++;
	}

	fprintf(fout, "total framenum:%d  ", srcframenum);
	fprintf(fout, "MATCH!\n");

	fclose(fin_1);
	fclose(fin_2);
	fclose(fout);

	free(buffer1);
	buffer1 = NULL;
	free(buffer2);
	buffer2 = NULL;

	return 0;
}
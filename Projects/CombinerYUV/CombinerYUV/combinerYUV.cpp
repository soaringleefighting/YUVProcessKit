/************************************************************************
//Function: horizontal combiner of two yuv image , yuv support yuv420p
//Author&Date: SoaringLee 3rd June 2018
//Modified:
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	int i, j, width, height, frame_size,framenum = 0;
	FILE *fin_1, *fin_2, *fout;
	unsigned char *y1, *u1, *v1, *y2, *u2, *v2, *y, *u, *v, *tmp_y1, *tmp_u1, *tmp_v1, *tmp_y2, *tmp_u2, *tmp_v2, *tmp_y, *tmp_u, *tmp_v;

	for (i = 0; i < argc; i++)
	{
		printf("Input parameters: %s \n", argv[i]);
	}
	if (argc < 6)
	{
		printf("\nInput parameters insufficient!!!\nUsage: combinerYUV.exe input1 input2 output width height\n\n");
		system("pause");
		return -1;
	}

	fin_1 = fopen(argv[1], "rb");
	if (NULL == fin_1)
	{
		printf("ERROR: open %s fail\n", argv[1]);
		return -1;
	}
	fin_2 = fopen(argv[2], "rb");
	if (NULL == fin_2)
	{
		printf("ERROR: open %s fail\n", argv[2]);
		return -1;
	}
	fout = fopen(argv[3], "wb");
	if (NULL == fout)
	{
		printf("ERROR: open %s fail\n", argv[3]);
		return -1;
	}

	width = atoi(argv[4]);
	height = atoi(argv[5]);
	frame_size = width*height;

	y1 = (unsigned char *)malloc(frame_size * 3 / 2);//yuv1
	if (NULL == y1)
	{
		printf("ERROR: malloc y1 fail\n");
		return -1;
	}
	u1 = y1 + width*height;
	v1 = u1 + width*height / 4;

	y2 = (unsigned char *)malloc(frame_size * 3 / 2);//yuv2
	if (NULL == y2)
	{
		printf("ERROR: malloc y2 fail\n");
		return -1;
	}
	u2 = y2 + width*height;
	v2 = u2 + width*height / 4;
	
	y = (unsigned char *)malloc(frame_size * 3);//fp_out
	if (NULL == y)
	{
		printf("ERROR: malloc y fail\n");
		return -1;
	}
	u = y + width*height * 2;
	v = u + width*height / 2;

	while ((fread(y1, 1, frame_size*3/2, fin_1) == frame_size*3/2)  && (fread(y2, 1, frame_size*3/2, fin_2) == frame_size*3/2))
	{
		//++方法一：逐像素拷贝
// 		for (i = 0; i < height;++i)
// 		{
// 			for (j = 0; j < width; ++j)
// 			{
// 				y[i*width*2 + j] = y1[i*width + j];
// 				y[i*width*2 + width + j] = y2[i*width + j];
// 			}
// 		}
// 
// 		for (i = 0; i < height / 2;++i)
// 		{
// 			for (j = 0; j < width / 2;++j)
// 			{
// 				u[i*width + j] = u1[i*width / 2 + j];
// 				u[i*width + width/2 +j] = u2[i*width / 2 + j];
// 
// 				v[i*width + j] = v1[i*width / 2 + j];
// 				v[i*width + width/2 + j] = v2[i*width / 2 + j];
// 			}
// 		}
		//++方法二：逐行拷贝
		tmp_y1 = y1;
		tmp_u1 = u1;
		tmp_v1 = v1;
		tmp_y2 = y2;
		tmp_u2 = u2;
		tmp_v2 = v2;
		tmp_y = y;
		tmp_u = u;
		tmp_v = v;

		for (i = 0; i < height; ++i)
		{
			memcpy(tmp_y, tmp_y1, width);
			tmp_y += width;
			tmp_y1 += width;
			memcpy(tmp_y, tmp_y2, width);
			tmp_y += width;
			tmp_y2 += width;
		}
		for (i = 0; i < height / 2; ++i)
		{
			memcpy(tmp_u, tmp_u1, width / 2);
			tmp_u += width / 2;
			tmp_u1 += width / 2;
			memcpy(tmp_u, tmp_u2, width / 2);
			tmp_u += width / 2;
			tmp_u2 += width / 2;

			memcpy(tmp_v, tmp_v1, width / 2);
			tmp_v += width / 2;
			tmp_v1 += width / 2;
			memcpy(tmp_v, tmp_v2, width / 2);
			tmp_v += width / 2;
			tmp_v2 += width / 2;
		}

		fwrite(y, 1, frame_size * 3/2 * 2, fout);

		printf("%dth frame ok!!\n", framenum);
		framenum++;
	}
	printf("combinerYUV successfully!! ,total frames: %d\n",framenum);

	free(y1); 
	y1 = NULL;
	free(y2); 
	y2 = NULL;
	free(y); 
	y = NULL;
	fclose(fin_1);
	fclose(fin_2);
	fclose(fout);
	return 0;
}

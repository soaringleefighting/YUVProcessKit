#include "stdio.h"
#include "stdlib.h"
#include "string"

int main(int argc, char** argv)
{
	unsigned char* y;
	FILE* fp_input, *fp_out;
	int i, width, height, start, stop;
	int filelen, frames_no, picturesize, skip_size;
	char outname[100];

	if (argc < 6)
	{
		printf("Usage: extractYUV.exe  input.yuv  width height startframe stopframe\n\n");
#if WIN32				
		system("pause");
#endif
		return -1;
	}

	
    width = atoi(argv[2]);
    height = atoi(argv[3]);
	start = atoi(argv[4]);
	stop = atoi(argv[5]);
	sprintf(outname,"%s_%d_%d.yuv",argv[1],start,stop);

	fp_input = fopen(argv[1], "rb");
	if (NULL == fp_input)
	{
		printf("open %s fail!\n",argv[1]);
		return -1;
	}

	fp_out = fopen(outname, "wb");
	if (NULL == fp_out)
	{
		printf("open %s fail!\n",argv[1]);
		return -1;
	}

	picturesize = width*height*3/2;
	fseek(fp_input,0,SEEK_END);
	filelen = ftell(fp_input);
	frames_no = filelen / picturesize;
	
	skip_size = start * picturesize;
    fseek(fp_input, skip_size,SEEK_SET);

	y = (unsigned char*)malloc(picturesize*sizeof(unsigned char*));

	for (i = 0; i < (stop - start); i++)
	{
		if (fread(y, 1, picturesize, fp_input) == picturesize)
		{
			fwrite(y, 1, picturesize, fp_out);
		}
	}

	printf("extractYUV ok!!!,from %d to %d, total frames:%d\n",start, stop, frames_no);

	free(y);
	y = NULL;
	fclose(fp_input);
	fclose(fp_out);

	return 0;
}
/*  
* File:        scale.c  
* Description: scale YUV420 video frames with cubic, bilinear & cubic convolution interpolation algorithms  
*              highly optimized algorithm  
*/   
   
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <math.h> 
  
#define ABS(x) (x > 0 ? (x) : (-x))  
#define clip(a, min, max) (a > max ? max : (a > min ? a : min))  
#define INTEGER 1  
#define NO_CB	1//色度分量直接赋值为0
  

int gcd(int a, int b)  
{  
    if(b)   
    {  
        return gcd(b, a%b);  
    }  
    else    
    {  
        return a;  
    }  
}  
  
float S(float x)  
{  
    if(ABS(x) < 1.0)  
    {  
        //1-2*Abs(x)^2+Abs(x)^3  
        return (1.0 - 2.0*x*x + ABS(x*x*x));  
    }  
    else if(ABS(x) >= 1.0 && ABS(x) < 2)  
    {  
        //4-8*Abs(x)+5*Abs(x)^2-Abs(x)^3  
        return (4.0 - 8.0*ABS(x) + 5.0*x*x - ABS(x*x*x));  
    }  
    else  
    {  
        return 0;  
    }  
}  
  
/************************************************************** 
       
Matrix multiplication A x B = C 
Input: A[m x p], B[p x n] 
Output: C[m x n] 
 
***************************************************************/  
void MatrixMultiply(float *A, float *B, float *C, int m, int p, int n)  
{  
    int i,j,k;  
    float sum;  
    for ( i = 0; i < m; i++)  
    {  
        for ( j = 0; j < n; j++)  
        {  
            sum = 0;  
            for ( k = 0; k < p; k++)  
            {  
                sum = sum + A[i * p + k] * B[k * n + j];  
            }  
            C[i*n +j] = sum;  
        }  
    }  
}  
  
/************************************************************** 
       
Matrix multiplication A x B = C 
Input: A[m x p], B[p x n] 
Output: C[m x n] 
 
***************************************************************/  
void MatrixMultiply_Int(int *A, int *B, int *C, int m, int p, int n)  
{  
    int i,j,k;  
    int sum;  
    for ( i = 0; i < m; i++)  
    {  
        for ( j = 0; j < n; j++)  
        {  
            sum = 0;  
            for ( k = 0; k < p; k++)  
            {  
                sum += A[i * p + k] * B[k * n + j];  
            }  
            C[i*n +j] = sum;  
        }  
    }  
}  
  
  
int scale(unsigned char algorithm,  
           int input_pixel_format,   
           unsigned char *pYIn, int lineY,   
           unsigned char *pCbIn, int lineCb,   
           unsigned char *pCrIn, int lineCr,   
           int srcWidth, int srcHeight,   
           int outWidth, int outHeight,  
           int output_pixel_format,   
           unsigned char *pYOut,   
           unsigned char *pCbOut,   
           unsigned char *pCrOut)  
{  
    float rx = (float)outWidth/(float)srcWidth;  
    float ry = (float)outHeight/(float)srcHeight;  
    int x = 0, y = 0;  
    int xx = 0, yy = 0;  
    int outWidth_cr = outWidth / 2;  
    int outHeight_cr = outHeight / 2;  
    int srcWidth_cr = srcWidth / 2;  
    int srcHeight_cr = srcHeight / 2;  
    float c0, c1, c2, c3;  
    float u, v;  
    int uu, vv;  
    unsigned char *p = NULL, *p1 = NULL;  
    unsigned long long t1, t2, t3, t4;  
  
    float A[4], B[16], C[4], tmp1[4], tmp2[1];  
    int   AA[4], BB[16], CC[4], tt1[4], tt2;  
    int *SSx1=NULL, *SSy1=NULL;  
    int *SSx2=NULL, *SSy2=NULL;  
    int *SSx3=NULL, *SSy3=NULL;  
    int *SSx4=NULL, *SSy4=NULL;  
    int *C0=NULL, *C1=NULL, *C2=NULL, *C3=NULL;  
    unsigned char* pend = pYIn + lineY * srcHeight - 1;  
      
    int x_ratio = (int) (((srcWidth - 1) << 8) / outWidth);  
    int y_ratio = (int) (((srcHeight - 1) << 8) / outHeight);  
    int i, j;  
  
    int yr, y_dif, one_min_y_diff, y_diff;  
    int y_index, xr, x_diff, one_min_x_diff,  index, Mult1, Mult2, Mult3, Mult4;  
    int *T1=NULL, *T2=NULL, *T3=NULL, *T4=NULL;  
  
    // Rational ratio  
    int gcdw = gcd(outWidth, srcWidth);  
    int gcdh = gcd(outHeight, srcHeight);  
    int rx_num = outWidth/gcdw;  
    int rx_den = srcWidth/gcdw;  
    int ry_num = outHeight/gcdh;  
    int ry_den = srcHeight/gcdh;  
  
    switch(algorithm)  
    {  
        case 0:  
            // cubic  
            for(y=0; y<outHeight; y++)  
            {  
                for(x=0; x<outWidth; x++)  
                {  
                    //g(x, y) = f(x/r, y/r) = f(x*rx_den/rx_num, y*ry_den/ry_num)  
                    xx = clip((int)((float)x / rx), 0, srcWidth-1);  
                    yy = clip((int)((float)y / ry), 0, srcHeight-1);  
                    pYOut[y*outWidth+x] = pYIn[yy*lineY + xx];  
                }  
            }  
            for(y=0; y<outHeight_cr; y++)  
            {  
                for(x=0; x<outWidth_cr; x++)  
                {  
                    //g(x, y) = f(x/r, y/r) = f(x*rx_den/rx_num, y*ry_den/ry_num)  
                    xx = clip((int)((float)x / rx), 0, srcWidth_cr-1);  
                    yy = clip((int)((float)y / ry), 0, srcHeight_cr-1);  
#if NO_CB
		    pCbOut[y*outWidth_cr+x] = 0;
#else
                    pCbOut[y*outWidth_cr+x] = pCbIn[yy*lineCb + xx];  
#endif
                }  
            }  
            for(y=0; y<outHeight_cr; y++)  
            {  
                for(x=0; x<outWidth_cr; x++)  
                {  
                    //g(x, y) = f(x/r, y/r) = f(x*rx_den/rx_num, y*ry_den/ry_num)  
                    xx = clip((int)((float)x / rx), 0, srcWidth_cr-1);  
                    yy = clip((int)((float)y / ry), 0, srcHeight_cr-1);  
#if NO_CB
 		    pCrOut[y*outWidth_cr+x] = 0;
#else
                    pCrOut[y*outWidth_cr+x] = pCrIn[yy*lineCr + xx];  
#endif
                }  
            }  
            break;  
        case 1:  
            // bilinear  
            for(y=0; y<outHeight; y++)  
            {  
                for(x=0; x<outWidth; x++)  
                {  
                    //g(x, y) = (1-c0)(1-c2)f(xx,yy) + (1-c2)(1-c1)f(xx,yy+1) + (1-c1)(1-c3)f(xx+1,yy) + (1-c0)(1-c3)f(xx+1,yy+1)  
                    c0 = (((float)y / ry) - (int)((float)y / ry));  
                    c1 = ((int)((float)y / ry) + 1 - ((float)y / ry));  
                    c2 = (((float)x / rx) - (int)((float)x / rx));  
                    c3 = ((int)((float)x / rx) + 1 - ((float)x / rx));  
                    xx = clip((int)((float)x / rx), 0, srcWidth-1);  
                    yy = clip((int)((float)y / ry), 0, srcHeight-1);  
                    pYOut[y*outWidth+x] = (float)((1.0-c0)*(1.0-c2)*pYIn[yy*lineY+xx]   
                                                + (1.0-c1)*(1.0-c2)*pYIn[clip(yy+1,0,srcHeight-1)*lineY+xx]   
                                                + (1.0-c1)*(1.0-c3)*pYIn[yy*lineY+clip(xx,0,srcWidth-1)]  
                                                + (1.0-c0)*(1.0-c3)*pYIn[clip(yy+1,0,srcHeight-1)*lineY+clip(xx,0,srcWidth-1)]);  
                }  
            }  
            for(y=0; y<outHeight_cr; y++)  
            {  
                for(x=0; x<outWidth_cr; x++)  
                {  
                    c0 = (((float)y / ry) - (int)((float)y / ry));  
                    c1 = ((int)((float)y / ry) + 1 - ((float)y / ry));  
                    c2 = (((float)x / rx) - (int)((float)x / rx));  
                    c3 = ((int)((float)x / rx) + 1 - ((float)x / rx));  
                    xx = clip((int)((float)x / rx), 0, srcWidth_cr-1);  
                    yy = clip((int)((float)y / ry), 0, srcHeight_cr-1); 
#if NO_CB
                    pCbOut[y*outWidth_cr+x] = (float)((1.0-c0)*(1.0-c2)*pCbIn[yy*lineCb+xx]   
                                                + (1.0-c1)*(1.0-c2)*pCbIn[clip(yy+1,0,srcHeight_cr-1)*lineCb+xx]   
                                                + (1.0-c1)*(1.0-c3)*pCbIn[yy*lineCb+clip(xx,0,srcWidth_cr-1)]  
                                                + (1.0-c0)*(1.0-c3)*pCbIn[clip(yy+1,0,srcHeight_cr-1)*lineCb+clip(xx,0,srcWidth_cr-1)]); 
#else
		   pCbOut[y*outWidth_cr+x] = 0;
#endif 
                }  
            }  
            for(y=0; y<outHeight_cr; y++)  
            {  
                for(x=0; x<outWidth_cr; x++)  
                {  
                    c0 = (((float)y / ry) - (int)((float)y / ry));  
                    c1 = ((int)((float)y / ry) + 1 - ((float)y / ry));  
                    c2 = (((float)x / rx) - (int)((float)x / rx));  
                    c3 = ((int)((float)x / rx) + 1 - ((float)x / rx));  
                    xx = clip((int)((float)x / rx), 0, srcWidth_cr-1);  
                    yy = clip((int)((float)y / ry), 0, srcHeight_cr-1);  
#if NO_CB
                    pCrOut[y*outWidth_cr+x] = (float)((1.0-c0)*(1.0-c2)*pCrIn[yy*lineCr+xx]   
                                                + (1.0-c1)*(1.0-c2)*pCrIn[clip(yy+1,0,srcHeight_cr-1)*lineCr+xx]   
                                                + (1.0-c1)*(1.0-c3)*pCrIn[yy*lineCr+clip(xx,0,srcWidth_cr-1)]  
                                                + (1.0-c0)*(1.0-c3)*pCrIn[clip(yy+1,0,srcHeight_cr-1)*lineCr+clip(xx,0,srcWidth_cr-1)]);  
#else
		    pCrOut[y*outWidth_cr+x] = 0;
#endif
                }  
            }  
            break;  
        default:  
            printf("Not supported scaling algorithm\n");  
            return -1;  
    }    
      
    return 0;  
}  
int main(int argc, char** argv)  
{  
    int i, j;  
    int w, h, tw, th;  
    FILE* fin=NULL, *fout=NULL;  
    unsigned char* src=NULL;  
    unsigned char* dst=NULL;  
    int algorithm = 0; 
    int frameidx = 0; 
  
    if(argc != 8)  
    {  
        printf("Usage: <%s> <in file> <out file> <src width> <src height> <target width> <target height> <algorithm>\n", argv[0]);  
        printf("Algorithm list: 0--cubic, 1--bilinear\n");  
#if WIN32		
		system("pause");
#endif
       return -1;  
    }  
  
    fin = fopen(argv[1], "rb");  
    fout = fopen(argv[2], "wb");  
    if(!fin || !fout)  
    {  
        printf("File Open Fail!\n");  
        return -1;  
    }  
    w = atoi(argv[3]);  
    h = atoi(argv[4]);  
    tw = atoi(argv[5]);  
    th = atoi(argv[6]);  
    algorithm = atoi(argv[7]);  
    src = (unsigned char*)malloc(w*h*3/2);  
    dst = (unsigned char*)malloc(tw*th*3/2);  
    memset(dst, 0, tw*th);  
    memset(dst, 0, tw*th/2);  
   
    fseek(fin, 0, SEEK_SET);  
    while( fread(src, 1, w*h*3/2, fin)!= 0)
   {
       // scale   
       scale(algorithm, 420, src, w, src+w*h, w/2, src+w*h*5/4, w/2, w, h, tw, th, 420, dst, dst+tw*th, dst+tw*th*5/4);  
  
       fwrite(dst, 1, tw*th*3/2, fout); 
       printf("%d successful\n", frameidx++);
   }
 
  
 
    if(fin)fclose(fin);  
    if(fout)fclose(fout);  
    if(src)free(src);  
    if(dst)free(dst);  
    return 0;  
}  

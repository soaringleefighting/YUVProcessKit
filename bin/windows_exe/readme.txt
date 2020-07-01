//*****************************************
// Function	  : readme.txt
// Description: executable file of YUV process kit for windows platform
// Modified   : created by lipeng 2020.7.1
//******************************************

1、YUV水平拼接功能：CombinerYUV.exe
Usage: combinerYUV.exe input1 input2 output width height

2、YUV文件比较功能：CompareYUV.exe
Usage: CompareYUV.exe yuv0 yuv1 width height

3、YUV文件拼接功能：ConnectYUV.exe
Usage: connectYUV.exe src2file src2file dstfile width height len1 len2

4、YUV文件剪切功能：ExtractYUV.exe
Usage: extractYUV.exe  input.yuv  width height startframe stopframe

5、YUV文件分割（帧率扩倍）: SplitYUV.exe
Usage: splitYUV.exe  srcyuv dstyuv1 dstyuv2 width height

6、YUV缩放功能: ScaleYUV.exe
Usage: <ScaleYUV.exe> <in file> <out file> <src width> <src height> <target width> <target height> <algorithm>
Algorithm list: 0--cubic, 1--bilinear
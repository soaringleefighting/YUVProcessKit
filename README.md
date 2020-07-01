# YUVProcessKit

YUVProcessKit is a series of projects for Processing YUV sequences, such as horizontal combiner, compare, connect, extract, scale and split and so on.

## 1、参考

【Codecs系列】YUV处理工具系列汇总： https://blog.csdn.net/SoaringLee_fighting/article/details/95807355


## 2、支持功能

说明：当前支持下面6个功能。

### 2.1、YUV水平拼接 CombinerYUV

YUV按照y,u,v三个分量顺序排列，支持YUV420P（I420 or YV12）采样格式。

Usage: combinerYUV.exe input1 input2 output width height

### 2.2、YUV文件比较 CompareYUV

比较结果写入文件compare_result.txt中。

Usage: CompareYUV.exe yuv0 yuv1 width height

### 2.3、YUV文件拼接 ConnectYUV

可以设置两个YUV文件的帧数，将两个YUV文件的三个分量顺序拼接，只支持YUV420P采样格式。

Usage: connectYUV.exe src2file src2file dstfile width height len1 len2

### 2.4、YUV文件剪切 ExtractYUV

实现YUV文件剪切，可以设置开始帧和结束帧。

Usage: extractYUV.exe  input.yuv  width height startframe stopframe

### 2.5、YUV文件分割（帧率扩倍） SplitYUV

实现YUV文件分割为两个子YUV，隔帧写入YUV，实现帧率扩倍功能。

Usage: splitYUV.exe  srcyuv dstyuv1 dstyuv2 width height

### 2.6、YUV缩放 ScaleYUV

实现YUV文件的缩放，插值算法可选择cublic立方插值和bilinear双线性插值。

Usage: <ScaleYUV.exe> <in file> <out file> <src width> <src height> <target width> <target height> <algorithm>
Algorithm list: 0--cubic, 1--bilinear


## 3、目录结构说明

bin			: windows平台下的可执行文件

Projects	: YUV处理工具的windows平台vs2013工程 


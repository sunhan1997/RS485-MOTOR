#include <stdio.h>
#include <math.h>


int main()
{
  int i;
  int  n=10;
	double x[7] ;
	double y[7] ;
	double z[7] ;
	FILE* fp;
	/* 打开立体定点的数据文件 data.txt -- 必须与程序在同一个目录下，否则需指明具体路径 */
	fp=fopen("wps.txt", "r");
		/* 逐个定点的读取x, y, z的坐标，数据文件的每一行是一个顶点的坐标，以逗号分隔 */
    for (i = 0; i < n; i++)	fscanf(fp, "%lf,", &x[i]);
	for (i = 0; i < n; i++)	fscanf(fp, "%lf,", &y[i]);
	for (i = 0; i < n; i++)	fscanf(fp, "%lf,", &z[i]);
	/* 关闭文件 */
	 fclose(fp); 
}
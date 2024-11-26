#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>
#include "OLED_Data.h"

/*参数宏定义*********************/

/*FontSize参数取值*/
/*此参数值不仅用于判断，而且用于计算横向字符偏移，默认值为字体像素宽度*/
#define OLED_8X16 8
#define OLED_6X8 6

/*IsFilled参数数值*/
#define OLED_UNFILLED 0
#define OLED_FILLED 1

/*********************参数宏定义*/


/*函数声明*********************/

/*初始化函数*/

/*更新函数*/
/* 将内存中的显示内容刷新到 OLED 屏幕上 */
/* 初始化 OLED 显示屏，配置必要的硬件和默认显示设置 */
void OLED_Init(void);

/* 将内存中的显示内容刷新到 OLED 屏幕上 */
void OLED_Update(void);

/* 刷新指定区域的显示内容，更新范围由 (X, Y) 坐标和宽高决定 */
void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*********************************************************************************/
/*********************************************************************************/

/* 清空整个显示屏，将显存设置为全黑 */
void OLED_Clear(void);

/* 清空显示屏的指定区域，区域由 (X, Y) 和宽高定义 */
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/* 反转整个显示屏的显示内容（像素反转：亮变暗，暗变亮） */
void OLED_Reverse(void);

/* 反转指定区域的显示内容，区域由 (X, Y) 和宽高定义 */
void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);


/************************************************************************/
/************************************************************************/
/*显示函数*/
/* 在指定位置 (X, Y) 显示一个字符，字体大小由 FontSize 决定 */
void OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize);

/* 在指定位置 (X, Y) 显示字符串，字体大小由 FontSize 决定 */
void OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize);

/* 在指定位置 (X, Y) 显示一个无符号数字，字体大小由 FontSize 决定，长度由 Length 指定 */
void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);

/* 在指定位置 (X, Y) 显示一个有符号数字，字体大小由 FontSize 决定，长度由 Length 指定 */
void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);

/* 在指定位置 (X, Y) 显示一个十六进制数字，字体大小由 FontSize 决定，长度由 Length 指定 */
void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);

/* 在指定位置 (X, Y) 显示一个二进制数字，字体大小由 FontSize 决定，长度由 Length 指定 */
void OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);

/* 在指定位置 (X, Y) 显示一个浮点数，字体大小由 FontSize 决定，整数和小数位数分别由 IntLength 和 FraLength 指定 */
void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);

/* 在指定位置 (X, Y) 显示图像，图像数据由 Image 指针提供，宽度和高度由 Width 和 Height 指定 */
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);

/* 在指定位置 (X, Y) 格式化并显示字符串，字体大小由 FontSize 决定 */
void OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...);

/****************************************************************************************/
/****************************************************************************************/
/*绘图函数*/
/* 在指定位置绘制一个点 */
void OLED_DrawPoint(int16_t X, int16_t Y);

/* 获取指定位置的点的状态（例如，是否为已点亮） */
uint8_t OLED_GetPoint(int16_t X, int16_t Y);

/* 绘制一条直线，连接 (X0, Y0) 和 (X1, Y1) */
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);

/* 绘制一个矩形，指定起点 (X, Y)，宽度和高度，是否填充 */
void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);

/* 绘制一个三角形，指定三个顶点的坐标 (X0, Y0), (X1, Y1), (X2, Y2)，是否填充 */
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);

/* 绘制一个圆，指定圆心 (X, Y) 和半径，是否填充 */
void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled);

/* 绘制一个椭圆，指定椭圆中心 (X, Y)，长半轴 A，短半轴 B，是否填充 */
void OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);

/* 绘制一个圆弧，指定圆心 (X, Y)，半径，起始角度和终止角度，是否填充 */
void OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);

/*********************函数声明*/

#endif


/*****************江协科技|版权所有****************/
/*****************jiangxiekeji.com*****************/

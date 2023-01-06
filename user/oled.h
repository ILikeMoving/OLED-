#ifndef __OLED_H__
#define __OLED_H__

#include "stm32f1xx_hal.h"	//����HAL��
#include "soft_iic.h"       //���IIC


/* ���ƺ� */
#define LEFT 			0x27
#define RIGHT 			0x26
#define UP 				0X29
#define DOWM 			0x2A
#define ON				0xA7
#define OFF				0xA6


/* BMPͼƬ���� 
	ͼƬ��ʽΪ��λ���飬�±�ֱ��ӦͼƬ�Ŀ�͸ߣ�
		BMP_xx[H/8][L];
*/
extern const uint8_t BMP_Picture[32/8][32];

/* ��ͷ�������� */
extern const uint8_t Test_BMP[48/8][24];

/* ���ֹ���ʱ�ӵ�BMP�ز�ͼƬ */
extern const uint8_t Scroll_Digit_BMP[240/8][20];          //������
extern const uint8_t Scroll_Digit_Small_BMP[160/8][14];    //С����
extern const uint8_t Colon_BMP[24/8][4];                   //ð��


/* ����������״̬ */
typedef enum 
{
	SET_PIXEL = 0x01,
  RESET_PIXEL = 0x00, 
} PixelStatus;


/* ���ܺ������� */
//д���ݣ����IICʹ��
void Soft_I2C_WriteByte(uint8_t addr,uint8_t data);
//д����
void WriteCmd(uint8_t IIC_Command);
//д����
void WriteDat(uint8_t IIC_Data);
//��ʼ��OLED
void OLED_Init(void);
//������ɱ�
void OLED_ON(void);
//�رյ�ɱ�
void OLED_OFF(void);
//ˢ�»��������ݵ�GDDRAM
void OLED_RefreshRAM(void);
//ˢ�²��ֻ��������ݵ�����GDDRAM
void OLED_RefreshPartRAM(uint8_t Page_Start, uint8_t Page_Stop, uint8_t Column_Start, uint8_t Column_Stop);
//������ݻ�����OLED_RAM buffer
void OLED_ClearRAM(void);
//ȫ�����
void OLED_FullyFill(uint8_t fill_Data);
//����
void OLED_FullyClear(void);
//�����������ص�����
void OLED_SetPixel(int16_t x, int16_t y, uint8_t set_pixel);
//����������ص�����
PixelStatus OLED_GetPixel(int16_t x, int16_t y);

/* ��ʾָ���ַ���ͼƬʱ��Ҫ�ֶ�ˢ�»�������GDDRAM 
* function list: OLED_ShowStr\OLED_ShowCN\OLED_Show_MixedCH\OLED_DrawBMP
*/
//��ʾӢ���ַ���
void OLED_ShowStr(int16_t x, int16_t y, uint8_t ch[], uint8_t TextSize);
//��ʾ�����ַ���
void OLED_ShowCN(int16_t x, int16_t y, uint8_t* ch);
//��ʾ��Ӣ�Ļ������
void OLED_ShowMixedCH(int16_t x, int16_t y, uint8_t* ch);
//��ʾͼƬ
void OLED_DrawBMP(int16_t x0,int16_t y0,int16_t L,int16_t H,const uint8_t BMP[]);

//�������
void OLED_AreaFill(int16_t x0,int16_t y0,int16_t L,int16_t H, uint8_t fill_data);
//�������
void OLED_AreaClear(int16_t x0,int16_t y0,int16_t L,int16_t H);
//ȫ���л���ʾ
void OLED_FullyToggle(void);
//�����л���ʾ
void OLED_AreaToggle(int16_t x0,int16_t y0,int16_t L,int16_t H);
//ȫ����ֱ��������
void OLED_VerticalShift(void);
//ȫ��ˮƽ��������
void OLED_HorizontalShift(uint8_t direction);
//ȫ��ͬʱ��ֱ��ˮƽ��������
void OLED_VerticalAndHorizontalShift(uint8_t direction);
//��Ļ����ȡ����ʾ
void OLED_DisplayMode(uint8_t mode);
//��Ļ���ȵ���
void OLED_IntensityControl(uint8_t intensity);

//����������ص����ݣ�����Ϊ������������
uint8_t GetPixel_For_Scroll(int16_t x, int16_t y, const uint8_t BMP[], uint8_t W);
//�����������ص����ݣ�����Ϊ������������
void SetPixel_For_Scroll(int16_t X, int16_t Y, int16_t x, int16_t y, uint8_t set_pixel);
//��bmp��ͼƬ�л�ȡСͼƬ��Ϊ����������һ֡ͼƬ
void Draw_BMP_For_Scroll(uint16_t x1, uint16_t y1, const uint8_t BMP[], uint16_t Y, uint8_t W, uint8_t H, uint16_t end_line);


#endif /* __OLED_H__ */


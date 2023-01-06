/***********************************
	�������ļ�������HAL��汾
***********************************/
#include "codetab.h"	//�ֿ��ļ�
#include "oled.h"		//����


//oled��ʾ�ߴ�
uint16_t const displayWidth                = 128;
uint16_t const displayHeight               = 64;

/*  OLED�Դ�
[0]0 1 2 3 ... 127	
[1]0 1 2 3 ... 127	
[2]0 1 2 3 ... 127	
[3]0 1 2 3 ... 127	
[4]0 1 2 3 ... 127	
[5]0 1 2 3 ... 127	
[6]0 1 2 3 ... 127	
[7]0 1 2 3 ... 127 */
static uint8_t OLED_RAM[8][128];//����GDDRAM������

void Soft_I2C_WriteByte(uint8_t addr,uint8_t data)
{
	Soft_IIC_Start();
	Soft_IIC_SendByte(0x78);         //�ӻ���ַ���дλ
	Soft_IIC_ReceiveACK();
	Soft_IIC_SendByte(addr);
	Soft_IIC_ReceiveACK();
	Soft_IIC_SendByte(data);
	Soft_IIC_ReceiveACK();
	Soft_IIC_Stop();
}

/**************************************************************
	 Prototype      : void WriteCmd(uint8_t IIC_Command)
	 Parameters     : IIC_Command
	 return					: none
	 Description    : д����
***************************************************************/
void WriteCmd(uint8_t IIC_Command)
{
	Soft_I2C_WriteByte(0x00, IIC_Command);
}

/**************************************************************
	 Prototype      : void WriteDat(uint8_t IIC_Data)
	 Parameters     : IIC_Data
	 return					: none
	 Description    : д����
***************************************************************/
void WriteDat(uint8_t IIC_Data)
{
	Soft_I2C_WriteByte(0x40, IIC_Data);
}

/**************************************************************
	 Prototype      : void OLED_Init(void)
	 Parameters     : none
	 return					: none
	 Description    : ��ʼ��OLEDģ��
***************************************************************/
void OLED_Init(void) 
{
	HAL_Delay(500);
	
	WriteCmd(0xAE); //����ʾ
	WriteCmd(0x20);	//�����ڴ�Ѱַģʽ	
	
	WriteCmd(0x00);	//00��ˮƽѰַģʽ;01����ֱѰַģʽ;10��ҳ��Ѱַģʽ(����);11����Ч
	WriteCmd(0x21); //�����п�ʼ�ͽ�����ַ
	WriteCmd(0x00); //����ʼ��ַ,��Χ:0 �C 127d (Ĭ��ֵ = 0d)
	WriteCmd(0x7F); //�н�����ַ,��Χ:0 �C 127d (Ĭ��ֵ = 127d)
	WriteCmd(0x22); //ҳ�����ÿ�ʼ�ͽ�����ַ
	WriteCmd(0x00); //ҳ����ʼ��ַ,��Χ:0-7d (Ĭ��ֵ= 0d)
	WriteCmd(0x07); //ҳ�������ַ,��Χ:0-7d (Ĭ��ֵ= 7d)

	WriteCmd(0xc8);	//����COM���ɨ�跽��
	WriteCmd(0x40); //--������ʼ�е�ַ
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
	WriteCmd(0xa1); //--���ö�����ӳ��0��127
	WriteCmd(0xa6); //--����������ʾ
	WriteCmd(0xa8); //--���ø��ñ�(1 ~ 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,�����ѭRAM����;0xa5,Output����RAM����
	WriteCmd(0xd3); //-������ʾ����
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--������ʾʱ�ӷ�Ƶ/����Ƶ��
	WriteCmd(0xf0); //--���÷���
	WriteCmd(0xd9); //--����pre-chargeʱ��
	WriteCmd(0x22); //
	WriteCmd(0xda); //--����com��ͷ��Ӳ������
	WriteCmd(0x12);
	WriteCmd(0xdb); //--����vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--����DC-DC
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--��oled���
	
	OLED_FullyClear();//����
}

/**************************************************************
	 Prototype      : void OLED_ON(void)
	 Parameters     : none
	 return					: none
	 Description    : ��OLED�������л���
***************************************************************/
void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}

/**************************************************************
	 Prototype      : void OLED_OFF(void)
	 Parameters     : none
	 return					: none
	 Description    : ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
***************************************************************/
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ�
	WriteCmd(0XAE);  //OLED����
}

/**************************************************************
	 Prototype      : void OLED_RefreshRAM(void)
	 Parameters     : none
	 return					: none
	 Description    : ȫ�����
***************************************************************/
void OLED_RefreshRAM(void)
{
	for(uint16_t m = 0; m < displayHeight/8; m++)
	{
		for(uint16_t n = 0; n < displayWidth; n++)
		{
				WriteDat(OLED_RAM[m][n]);
		}
	}
}

/**************************************************************
	 Prototype      : void OLED_RefreshRAM_Test(void)
	 Parameters     : Page_Start    ҳ��ʼ��ַ(��СΪ0)
     Parameters     : Page_Stop     ҳ������ַ(���Ϊ7)
     Parameters     : Column_Start  �п�ʼ��ַ(��СΪ0)
     Parameters     : Column_Stop   �н�����ַ(���Ϊ127)
	 return			: none
	 Description    : �������
***************************************************************/
void OLED_RefreshPartRAM(uint8_t Page_Start, uint8_t Page_Stop, uint8_t Column_Start, uint8_t Column_Stop)
{
	WriteCmd(0X21);  //�����е�ַ
	WriteCmd(Column_Start);  //�п�ʼ��ַ
	WriteCmd(Column_Stop);  //�н�����ַ
	
	WriteCmd(0X22);  //����ҳ��ַ
	WriteCmd(Page_Start);  //ҳ��ʼ��ַ
	WriteCmd(Page_Stop);  //ҳ������ַ
	
    for(uint16_t m = Page_Start; m < (Page_Stop + 1); m++)
	{
		for(uint16_t n = Column_Start; n < (Column_Stop + 1); n++)
		{
				WriteDat(OLED_RAM[m][n]);
		}
	}
}

/**************************************************************
	 Prototype      : void OLED_ClearRAM(void)
	 Parameters     : none
	 return					: none
	 Description    : ������ݻ�����
***************************************************************/
void OLED_ClearRAM(void)
{
	for(uint16_t m = 0; m < displayHeight/8; m++)
	{
		for(uint16_t n = 0; n < displayWidth; n++)
		{
				OLED_RAM[m][n] = 0x00;
		}
	}
}


/**************************************************************
	 Prototype      : void OLED_Fill(uint8_t fill_Data)
	 Parameters     : fill_Data ����1�ֽ�����
	 return					: none
	 Description    : ȫ����� 0x00~0xff
***************************************************************/
void OLED_FullyFill(uint8_t fill_Data)
{
	for(uint16_t m = 0; m < displayHeight/8; m++)
	{
		for(uint16_t n = 0; n < displayWidth; n++)
		{
				OLED_RAM[m][n] = fill_Data;
		}
	}
	
	OLED_RefreshRAM();
}

/**************************************************************
	 Prototype      : void OLED_FullyClear(void)
	 Parameters     : none
	 return					: none
	 Description    : ȫ�����
***************************************************************/
void OLED_FullyClear(void)
{
		OLED_FullyFill(RESET_PIXEL);
}

/**************************************************************
	Prototype      :  void OLED_SetPixel(int16_t x, int16_t y, uint8_t set_pixel)
	Parameters     : 	x,y -- ��ʼ������(x:0~127, y:0~63); 
										set_pixel	 �õ������  SET_PIXEL = 1, RESET_PIXEL = 0
	return				 :  none
	Description    : 	�����������ص�����
***************************************************************/
void OLED_SetPixel(int16_t x, int16_t y, uint8_t set_pixel)
{ 
	if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight) {
		if(set_pixel){
				OLED_RAM[y/8][x] |= (0x01 << (y%8));
		}  
		else{
				OLED_RAM[y/8][x] &= ~(0x01 << (y%8));
		}
	}
}

/**************************************************************
	Prototype      :  void OLED_GetPixel(int16_t x, int16_t y)
	Parameters     : 	x,y -- ��ʼ������(x:0~127, y:0~63); 
	return				 :  PixelStatus ���ص�״̬ 	SET_PIXEL = 1, RESET_PIXEL = 0
	Description    : 	����������ص�����	
***************************************************************/
PixelStatus OLED_GetPixel(int16_t x, int16_t y)
{
	 if(OLED_RAM[y/8][x] >> (y%8) & 0x01)
		 return SET_PIXEL;
	 
	return	RESET_PIXEL;
}

/**************************************************************
	Prototype      : void OLED_ShowStr(int16_t x, int16_t y, uint8_t ch[], uint8_t TextSize)
	Parameters     : 	x,y -- ��ʼ������(x:0~127, y:0~63); 
										ch[] -- Ҫ��ʾ���ַ���; 
										TextSize -- �ַ���С(1:6*8 ; 2:8*16)
	return				 :  none
	Description    : 	��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
***************************************************************/
void OLED_ShowStr(int16_t x, int16_t y, uint8_t ch[], uint8_t TextSize)
{ 
	if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight) {
		int32_t c = 0;
		uint8_t j = 0;
	
		switch(TextSize)
		{
			case 1:
			{
				while(ch[j] != '\0')
				{
					c = ch[j] - 32;
					if(c < 0)	//��Ч�ַ�
						break;
					
					if(x >= 125 || (127-x < 6))//һ�������ʾ�ַ�����21�ֽ���ʾ,������У�����ʾ || ʣ����С��6������ʾ�����ַ���������ʾ
					{
						x = 0;
						y += 8;//������ʾ
						if(63 - y < 8)	// ��������ʾһ��ʱ����ʾ
							break;
					}
					for(uint8_t m = 0; m < 6; m++)
					{
						for(uint8_t n = 0; n < 8; n++)
						{
							OLED_SetPixel(x+m, y+n, (F6x8[c][m] >> n) & 0x01);
						}
					}
					x += 6;
					j++;
				}
			}break;
			case 2:
			{
				while(ch[j] != '\0')
				{
					c = ch[j] - 32;
					if(c < 0)	//��Ч�ַ�
						break;
					
					if(x >= 127 || (127-x < 8))//16�ֽ���ʾ || ʣ����С��8������ʾ�����ַ���������ʾ
					{
						x = 0;
						y += 16;//������ʾ
						if(63 - y < 16)	// ��������ʾһ��ʱ����ʾ
							break;
					}
					for(uint8_t m = 0; m < 2; m++)
					{
						for(uint8_t n = 0; n < 8; n++)
						{
							for(uint8_t i = 0; i < 8; i++)
							{
									OLED_SetPixel(x+n, y+i+m*8, (F8X16[c][n+m*8] >> i) & 0x01);
							}
						}	
					}
					x += 8;
					j++;
				}
			}break;
		}
	}
}

/**************************************************************
	 Prototype      : void OLED_ShowCN(int16_t x, int16_t y, uint8_t* ch)
	 Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); 
										CN[]:������codetab.h�е�����
	 return				  : none
	 Description    : ��ʾcodetab.h�еĺ���,16*16����
***************************************************************/
void OLED_ShowCN(int16_t x, int16_t y, uint8_t* ch)
{
	if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight) {
		int32_t  len = 0;
		
		while(ch[len] != '\0')
		{
			if(x >= 127 || (127-x < 16))//8��������ʾ||ʣ����С��16������ʾ�����ַ���������ʾ
			{
				x = 0;
				y += 16;
				if(63 - y < 16)	// ��������ʾһ��ʱ����ʾ
					break;
			}
					
			//��Ҫ�����������ݴ�����ʾ���ݵ�����
			for(uint8_t i = 0; i < sizeof(F16x16_CN)/sizeof(GB2312_CN); i++)
			{
				if(((F16x16_CN[i].index[0] == ch[len]) && (F16x16_CN[i].index[1] == ch[len+1]))){
						for(uint8_t m = 0; m < 2; m++)	//ҳ
						{
								for(uint8_t n = 0; n < 16; n++) // ��
								{
										for(uint8_t j = 0; j < 8; j++)	// ��
										{
											OLED_SetPixel(x+n, y+j+m*8, (F16x16_CN[i].encoder[n+m*16] >> j) & 0x01);
										}
								}
						}			
						x += 16;
						len += 2;
						break;
				}
				else if(F16x16_CN[i].index[0] == ch[len] && ch[len] == 0x20){
					for(uint8_t m = 0; m < 2; m++)
					{
							for(uint8_t n = 0; n < 16; n++)
							{
								for(uint8_t j = 0; j < 8; j++)
								{
									OLED_SetPixel(x+n, y+j+m*8, (F16x16_CN[i].encoder[n+m*16] >> j) & 0x01);
								}								
							}	
					}			
					x += 16;
					len++;
					break;
				}
			}
		}
	}
}

/**************************************************************
	 Prototype      : void OLED_Show_MixedCH(int16_t x, int16_t y, uint8_t* ch)
	 Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); CN[]:������codetab.h�е�����
	 return				  : none
	 Description    : ��ʾcodetab.h�еĺ���,16*16����,Ӣ��,8*16����
***************************************************************/
void OLED_ShowMixedCH(int16_t x, int16_t y, uint8_t* ch)
{
	if (x >= 0 && x < displayWidth && y >= 0 && y < displayHeight) {
		int32_t len = 0, c;

		while(ch[len] != '\0')
		{
			if(ch[len] >= 0xa1)//GB2312��0xA1A0��ʼ
			{
				for(uint8_t i = 0; i < sizeof(F16x16_CN)/sizeof(GB2312_CN); i++)
				{
					if(((F16x16_CN[i].index[0] == ch[len]) && (F16x16_CN[i].index[1] == ch[len+1])))
					{
						if(x >= 127|| (127-x < 16))//8��������ʾ||ʣ����С��16������ʾ�����ַ���������ʾ
						{
							x = 0;
							y += 16;
							if(63 - y < 16)	// ��������ʾһ��ʱ����ʾ
								break;
						}
						for(uint8_t m = 0; m < 2; m++)	//ҳ
						{
								for(uint8_t n = 0; n < 16; n++)	//��
								{
									for(uint8_t j = 0; j < 8; j++)	//��
									{
	
										OLED_SetPixel(x+n, y+j+m*8, (F16x16_CN[i].encoder[n+m*16] >> j) & 0x01);
									}		
								}								
						}			
						x += 16;
						len += 2;
						break;
					}
				}
			}
			else if(ch[len] <= 127)//ASCII���뷶Χ0-127
			{
				c = ch[len] - 32;
				if(c < 0)	// ��Ч�ַ�
					break;
				if(x >= 127 || (127-x < 8))//16�ֽ���ʾ || ʣ����С��8������ʾ�����ַ���������ʾ
				{
					x = 0;
					y += 16;
					if(63 - y < 16)	// ��������ʾһ��ʱ����ʾ
								break;
				}
				for(uint8_t m = 0; m < 2; m++)
				{
						for(uint8_t n = 0; n < 8; n++)
						{
							for(uint8_t i = 0; i < 8; i++)
							{
									OLED_SetPixel(x+n, y+i+m*8, (F8X16[c][n+m*8] >> i) & 0x01);
							}
						}
				}
				x += 8;
				len++;
			}
		}
	}
}

/***************************************************************
	 Prototype      :	void OLED_DrawBMP(int16_t x0,int16_t y0,int16_t L,int16_t H,const uint8_t BMP[])
	 Parameters     : (x0,y0)���곤L��H�������ͼ��BMP
										0<=x0<=127 0<=y0<=63 0<=L+x0<=127 0<=H+y0<= 63 ͼ��ȡģ ����ȡģ���ֽڵ���
	 return				  : none
	 Description    : ����ͼ����ƣ���ʾBMPλͼ,��ʽʹ�ö�ά����洢
***************************************************************/
void OLED_DrawBMP(int16_t x0,int16_t y0,int16_t L,int16_t H,const uint8_t BMP[])
{
	if (x0 >= 0 && x0 < displayWidth && x0+L <= displayWidth &&\
		y0 >= 0 && y0 < displayHeight && y0+H <= displayHeight) {
		
		uint8_t *p = (uint8_t *)BMP;
		for(int16_t y = y0; y < y0+H; y+=8)
		{
			for(int16_t x = x0; x < x0+L; x++)
			{
				for(int16_t i = 0; i < 8; i++)
				{
//					OLED_SetPixel(x, y+i, ((*((uint8_t *)BMP+(x-x0)+L*((y-y0)/8))) >> i) & 0x01);
						OLED_SetPixel(x, y+i, ((*p) >> i) & 0x01);
				}
				p++;
			}
		}
	}
}

/***************************************************************
	 Prototype      :	void OLED_AreaFill(int16_t x0,int16_t y0,int16_t L,int16_t H)
	 Parameters     : �������������(x0,y0)���곤L��H����
										0<=x0<=127 0<=y0<=63 0<=L+x0<=127 0<=H+y0<= 63 ͼ��ȡģ ����ȡģ���ֽڵ���
	 return				  : none
	 Description    : �涨�����������
***************************************************************/
void OLED_AreaFill(int16_t x0,int16_t y0,int16_t L,int16_t H, uint8_t fill_data)
{
	if (x0 >= 0 && x0 < displayWidth && x0+L <= displayWidth &&\
		y0 >= 0 && y0 < displayHeight && y0+H <= displayHeight) {
		
		for(int16_t y = y0; y < y0+H; y++)
		{
			for(int16_t x = x0; x < x0+L; x++)
			{
				for(int16_t i = 0; i < 8; i++)
					{
							OLED_SetPixel(x, y+i, (fill_data >> i) & SET_PIXEL);
					}
			}
		}
		OLED_RefreshRAM();
	}
}

/***************************************************************
	 Prototype      :	void OLED_AreaCLR(int16_t x0,int16_t y0,int16_t L,int16_t H)
	 Parameters     : (x0,y0)���곤L��H����
										0<=x0<=127 0<=y0<=63 0<=L+x0<=127 0<=H+y0<= 63 ͼ��ȡģ ����ȡģ���ֽڵ���
	 return				  : none
	 Description    : �涨�����������
***************************************************************/
void OLED_AreaClear(int16_t x0,int16_t y0,int16_t L,int16_t H)
{
	if (x0 >= 0 && x0 < displayWidth && x0+L <= displayWidth &&\
		y0 >= 0 && y0 < displayHeight && y0+H <= displayHeight) {
		
		for(int16_t y = y0; y < y0+H; y+=8)
		{
			for(int16_t x = x0; x < x0+L; x++)
			{
				for(int16_t i = 0; i < 8; i++)
					{
							OLED_SetPixel(x, y+i, RESET_PIXEL);
					}
			}
		}		
		//OLED_RefreshRAM();
	}
}

/***************************************************************
	 Prototype      :	void OLED_FullyToggle(void)
	 Parameters     : none
	 return				  : none
	 Description    : ����������ȡ����ˢ�µ�GDDRAM
***************************************************************/
void OLED_FullyToggle(void)
{
	for(uint16_t m = 0; m < displayHeight/8; m++)
	{
		for(uint16_t n = 0; n < displayWidth; n++)
		{
				OLED_RAM[m][n] = ~OLED_RAM[m][n];
		}
	}
	OLED_RefreshRAM();
}
/***************************************************************
	 Prototype      :	void OLED_AreaToggle(int16_t x0,int16_t y0,int16_t L,int16_t H)
	 Parameters     : (x0,y0)���곤L��H����
										0<=x0<=127 0<=y0<=63 0<=L+x0<=127 0<=H+y0<= 63 ͼ��ȡģ ����ȡģ���ֽڵ���
	 return				  : none
	 Description    : �涨��������ȡ��
***************************************************************/
void OLED_AreaToggle(int16_t x0,int16_t y0,int16_t L,int16_t H)
{
	if (x0 >= 0 && x0 < displayWidth && x0+L <= displayWidth &&\
		y0 >= 0 && y0 < displayHeight && y0+H <= displayHeight) {
		
		for(int16_t y = y0; y < y0+H; y+=8)
		{
			for(int16_t x = x0; x < x0+L; x++)
			{
				for(int16_t i = 0; i < 8; i++)
					{
							OLED_SetPixel(x, y+i, !OLED_GetPixel(x, y+i));
					}
			}
		}		
		OLED_RefreshRAM();
	}
}

/****************************************************************
	ȫ����ֱƫ��,0->63����
	����ֱ����,��Χ0-63
	����ֱ����,��Χ63-0
****************************************************************/
void OLED_VerticalShift(void)
{
	for(uint8_t i = 0; i < displayHeight; i++)
		{
			WriteCmd(0xd3);//������ʾƫ�ƣ�0->63����
			WriteCmd(i);//ƫ����
			HAL_Delay(40);//��ʱʱ��
		}
}

/****************************************************************
	��Ļ����ˮƽȫ����������
	��		LEFT	0x27
	��		RIGHT	0x26
****************************************************************/
void OLED_HorizontalShift(uint8_t direction)

{
	WriteCmd(direction);//���ù�������
	WriteCmd(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	WriteCmd(0x00);//���ÿ�ʼҳ��ַ
	WriteCmd(0x05);//����ÿ����������֮���ʱ������֡Ƶ
	WriteCmd(0x07);//���ý���ҳ��ַ
	WriteCmd(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	WriteCmd(0xff);//�����ֽ����ã�Ĭ��Ϊ0xff
	WriteCmd(0x2f);//��������-0x2f�����ù���-0x2e��������Ҫ��д����
}

/****************************************************************
	��Ļ���ݴ�ֱˮƽȫ����������
	��		UP		0x29
	��		DOWN	0x2A
****************************************************************/
void OLED_VerticalAndHorizontalShift(uint8_t direction)
{
	WriteCmd(direction);//���ù�������
	WriteCmd(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	WriteCmd(0x00);//���ÿ�ʼҳ��ַ
	WriteCmd(0x05);//����ÿ����������֮���ʱ������֡Ƶ
	WriteCmd(0x07);//���ý���ҳ��ַ
	WriteCmd(0x01);//��ֱ����ƫ����
	
	WriteCmd(0x2f);//��������-0x2f�����ù���-0x2e��������Ҫ��д����
}

/****************************************************************
	��Ļ����ȡ����ʾ
	��	ON	0xA7
	��	OFF	0xA6	Ĭ�ϴ�ģʽ���������ص���
****************************************************************/
void OLED_DisplayMode(uint8_t mode)
{
	WriteCmd(mode);
}

/****************************************************************
	��Ļ���ȵ���
	intensity	0-255
	Ĭ��Ϊ0x7f
****************************************************************/
void OLED_IntensityControl(uint8_t intensity)
{
	WriteCmd(0x81);
	WriteCmd(intensity);
}

/***************************************************************
	 Prototype      : uint8_t GetPixel_For_Scroll(int16_t x, int16_t y, const uint8_t BMP[], uint8_t W)
	 Parameters     : x        ������
     Parameters     : y        ������
     Parameters     : BMP[]    bmpͼƬ����ά���飩
     Parameters     : W        bmpͼƬ��ȣ����أ�
	 return			: ���������
	 Description    : ����������ص����ݣ�����Ϊ������������
***************************************************************/
uint8_t GetPixel_For_Scroll(int16_t x, int16_t y, const uint8_t BMP[], uint8_t W)
{
	uint8_t *p = (uint8_t *)BMP;
	p += x + (y/8)*W;
	if((*p) >> (y%8) & 0x01) return 1;
	return 0;
}

/***************************************************************
	 Prototype      : void SetPixel_for_ScrollDigit(int16_t X, int16_t Y, int16_t x, int16_t y, uint8_t set_pixel)
	 Parameters     : X                
     Parameters     : Y                
     Parameters     : x                
     Parameters     : y
     Parameters     : set_pixel        
	 return			: none
	 Description    : �����������ص����ݣ�����Ϊ������������
***************************************************************/
void SetPixel_For_Scroll(int16_t X, int16_t Y, int16_t x, int16_t y, uint8_t set_pixel)
{ 
		if(set_pixel)
		{
			OLED_RAM[(Y+y)/8][X+x] |= (0x01 << ((Y+y)%8));
		}  
		else
		{
			OLED_RAM[(Y+y)/8][X+x] &= ~(0x01 << ((Y+y)%8));
		}
}

/****************************************************************************************************************************************************
	 Prototype      : void Draw_Digit_BMP(uint16_t x1, uint16_t y1, const uint8_t BMP[], uint16_t Y,uint8_t W, uint8_t H,  uint16_t end_line)  
     Parameters     : x1                ȷ��ͼƬ��ʾλ�ã����Ͻ����ص�����꣩
     Parameters     : y1                ȷ��ͼƬ��ʾλ�ã����Ͻ����ص������꣩
     Parameters     : BMP[]             �ز�ͼƬ
     Parameters     : Y                 ��ѡ��һ֡ͼƬ���ز�ͼƬ�е�������
     Parameters     : W                 �ز�ͼƬ��ȣ�Ҳ��һ֡ͼƬ�Ŀ�ȣ�
     Parameters     : H                 һ֡ͼƬ�ĸ߶�
     Parameters     : end_line          ���ز�ͼƬ�л������һ��(���ڹ���ѭ������β���)
	 return			: none
	 Description    : ��bmp��ͼƬ�л�ȡСͼƬ��Ϊ����������һ֡ͼƬ
*****************************************************************************************************************************************************/
void Draw_BMP_For_Scroll(uint16_t x1, uint16_t y1, const uint8_t BMP[], uint16_t Y, uint8_t W, uint8_t H, uint16_t end_line)
{
	uint16_t x0,y0,y,Temp;
	
	for(y = Y , y0 = 0 ; y0 < H ; y++ , y0++)
		{
			if(y > end_line) y -= (end_line+1);
			for(x0 = 0; x0 < W ; x0++)
			{
				Temp = GetPixel_For_Scroll(x0, y, BMP, W);
				SetPixel_For_Scroll(x1,y1,x0,y0,Temp);
			}
		}
	  
}

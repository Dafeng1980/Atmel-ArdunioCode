/*
 * CFile1.c
 *
 * Created: 2017/7/18 13:50:23
 *  Author: PE_NPI
 */ 
// matrix2pcs20140821.ino

const int SH_CP = 8;
const int DS = 9;
const int ST_CP = 10;
//����4��������ʱ����
byte part_A[8],part_B[8],part_C[8],part_D[8],part_A_A[8],part_B_B[8],
part_C_C[8],part_D_D[8],part_E[8],part_F[8],part_G[8],part_H[8];

//�ַ������ݣ������Ƕ�ά����ĵ�1ά���������������󣬵ڶ�ά�е�0����͵�1����ֱ�����ߵ�8*8������ұߵ�8*8����

byte Col[][2][16]={
	{
		{0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFC,0x03,0xFD,0xFF,0xFD,0xFF,0xC0,0x0F,0xDF,0xEF},
		{0xDF,0xEF,0xDF,0xEF,0xC0,0x0F,0xFF,0xFF,0xDB,0x77,0xDD,0xBB,0xBD,0xBB,0x7F,0xFB},/*"��",0*/
	}
	,
	{
		{0xFF,0xBF,0x83,0xBF,0xBB,0xBF,0xB4,0x01,0xB7,0x7F,0xAF,0x5F,0xB6,0xDF,0xB6,0x03},
		{0xBB,0xDF,0xBB,0xDF,0xBB,0xDF,0x94,0x01,0xAF,0xDF,0xBF,0xDF,0xBF,0xDF,0xBF,0xDF},/*"��",1*/
	}
	,
	{
		{0xFF,0x7F,0xDF,0xBF,0xE8,0x03,0xEE,0xEF,0x7D,0xB7,0xBB,0x7B,0xBE,0xDF,0xEC,0x0F},
		{0xEF,0xEF,0xDF,0x77,0x1E,0xAF,0xDC,0xDF,0xDA,0xEF,0xD6,0xB7,0xDE,0x79,0xFE,0xFF},/*"��",2*/
	}
	,
	{
		{0xFF,0xFF,0xC0,0x07,0xDF,0xF7,0xDF,0xF7,0xC0,0x07,0xDB,0xEF,0xDD,0xDF,0xD0,0x07},
		{0xDD,0xDF,0xDD,0xDF,0xC0,0x03,0xDD,0xDF,0xBD,0xDF,0xBB,0xDF,0x7B,0xDF,0xF7,0xDF},/*"��",3*/
	}
	,
	{
		{0xDF,0xBF,0xC1,0x81,0xB7,0x6F,0x7F,0xFF,0xC0,0x07,0xDF,0xF7,0xC0,0x07,0xDF,0xF7},
		{0xC0,0x07,0xDF,0xF7,0xC0,0x07,0xF7,0xDF,0x00,0x01,0xF7,0xDF,0xEF,0xDF,0xDF,0xDF},/*"��",4*/
	}
	,
	{
		{0xFF,0xBF,0xDF,0xBF,0xEF,0xBF,0xEF,0xBF,0x78,0x03,0xBF,0xBF,0xBF,0xBF,0xEF,0xBF},
		{0xE0,0x01,0xDF,0xBF,0x1F,0x7F,0xDE,0xFF,0xDD,0xEF,0xDB,0xF7,0xD0,0x03,0xFB,0xFB},/*"��",5*/
	}
	,
};
byte Row[] = {1,2,4,8,16,32,64,128};
	
void setup() {
	pinMode(SH_CP, OUTPUT);
	pinMode(DS,OUTPUT);
	pinMode(ST_CP,OUTPUT);
}

//�������ʾ�ַ�����
byte ch_num=6;

//������ʾ�ٶȱ���
byte speed = 10;

void loop() {
	//��ʼ�������ƶ�������
	byte counter=0;
	//col[][]��ά�������������飬��Ϊ16�����ݣ�ѭ��16�Σ������ݷ������ʱ������4����ʱ������Ӧ4��8*8����
	//��Ӧ˳����part_A��Ӧ���ϣ�part_B��Ӧ���ϣ�part_C��Ӧ���£�
	//A��C�����������еļ������ݣ�B��C���������е�ż������
	for(int c=0; c<ch_num; c++){
		byte a=0;
		byte b=0;
		for(int i=0; i<16; i++){
			//i%2==0֤���������е�ż�����ݣ�����0%2==0����col������0,2,4,6,8,10,12,14,16��Ӧ����װ��part_A��part_C
			if(i%2==0)
			{
				//�����ȡ���ַ�����ʱ�ַ����ݿ�col�еĵ����ڶ�������ʱ��
				if(c==(ch_num-2)){
					part_A[a]=Col[c][0][i];
					part_C[a]=Col[c][1][i];
					//�ڶ�����������Ϊ������һ������
					part_A_A[a]=Col[c+1][0][i];
					part_C_C[a]=Col[c+1][1][i];
					//��������Ϊ�ַ��������п�ͷ����
					part_E[a]=Col[0][0][i];
					part_G[a]=Col[0][1][i];
					//��col[]�����е�������һ�������ʱ����
					a++;
				}
				//�����ȡ���ַ�����ʱ�ַ����ݿ�col�еĵ�����һ������ʱ��
				else if(c==(ch_num-1)){
					part_A[a]=Col[c][0][i];
					part_C[a]=Col[c][1][i];
					//ѭ�����ַ����ݿ�Ŀ�ͷ����
					part_A_A[a]=Col[0][0][i];
					part_C_C[a]=Col[0][1][i];
					part_E[a]=Col[1][0][i];
					part_G[a]=Col[1][1][i];
					//��col[]�����е�������һ�������ʱ����
					a++;
				}
				else
				//���������˼·�������ȡ�ַ�����
				{
					part_A[a]=Col[c][0][i];
					part_C[a]=Col[c][1][i];
					part_A_A[a]=Col[c+1][0][i];
					part_C_C[a]=Col[c+1][1][i];
					part_E[a]=Col[c+2][0][i];
					part_G[a]=Col[c+2][1][i];
					a++;
				}
			}
			//��i%2==0���Ӧ�����i%2��0����ִ������else���������
			else
			{
				//�����ȡ���ַ�����ʱ�ַ����ݿ�col�еĵ����ڶ����ַ�����ʱ
				if(c==(ch_num-2)){
					part_B[b]=Col[c][0][i];
					part_D[b]=Col[c][1][i];
					//�ڶ�����������Ϊ�ַ����ݿ������һ���ַ�����
					part_B_B[b]=Col[c+1][0][i];
					part_D_D[b]=Col[c+1][1][i];
					//Ϊ��ѭ����ʾ���������ݻص��ַ����ݿ⿪ͷ����
					part_F[b]=Col[0][0][i];
					part_H[b]=Col[0][1][i];
					//��col[]�����е�������һ�������ʱ����
					b++;
				}
				//�����ȡ���ַ�����ʱ����cΪ�ַ����ݿ�col�еĵ�����1���ַ�����ʱ
				else if(c==(ch_num-1)){
					part_B[b]=Col[c][0][i];
					part_D[b]=Col[c][1][i];
					part_B_B[b]=Col[0][0][i];
					part_D_D[b]=Col[0][1][i];
					part_F[b]=Col[1][0][i];
					part_H[b]=Col[1][1][i];
					b++;
				}
				//���else���������Ϊ����˼·�����ַ���ȡ����
				else {
					part_B[b]=Col[c][0][i];
					part_D[b]=Col[c][1][i];
					part_B_B[b]=Col[c+1][0][i];
					part_D_D[b]=Col[c+1][1][i];
					part_F[b]=Col[c+2][0][i];
					part_H[b]=Col[c+2][1][i];
					b++;
				}
			}
		}
		//��һ��dowhileѭ��������B�����ƶ�������A��λ�ã�����A_A�����ƶ�������B��λ�á�
		do{
			for(int t=0; t<speed; t++){
				for(int i=0; i<8; i++){
					digitalWrite(ST_CP, LOW);
					shiftOut(DS, SH_CP, MSBFIRST, part_A[i]<<counter|part_B[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, part_B[i]<<counter|part_A_A[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, part_A_A[i]<<counter|part_B_B[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, part_B_B[i]<<counter|part_E[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, Row[i]);
					shiftOut(DS, SH_CP, MSBFIRST, part_C[i]<<counter|part_D[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, part_D[i]<<counter|part_C_C[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, part_C_C[i]<<counter|part_D_D[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, part_D_D[i]<<counter|part_G[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, Row[i]);
					digitalWrite(ST_CP, HIGH);
				}
			}
			counter++;
		}
		//�ڶ���dowhileѭ��������A_A�����ƶ�������A��λ�ã�������B_B�������ƶ�������B��λ�ã��ڶ����������ֽ�����ʾ����
		while (counter<8);
		counter=0;
		do{
			for(int t=0; t<speed; t++){
				for(int i=0; i<8; i++){
					digitalWrite(ST_CP, LOW);
					shiftOut(DS, SH_CP, MSBFIRST, part_B[i]<<counter|part_A_A[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, part_A_A[i]<<counter|part_B_B[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, part_B_B[i]<<counter|part_E[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, part_E[i]<<counter|part_F[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, Row[i]);
					shiftOut(DS, SH_CP, MSBFIRST, part_D[i]<<counter|part_C_C[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, part_C_C[i]<<counter|part_D_D[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, part_D_D[i]<<counter|part_G[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, part_G[i]<<counter|part_H[i]>>(8-counter));
					shiftOut(DS, SH_CP, MSBFIRST, Row[i]);
					digitalWrite(ST_CP, HIGH);
				}
			}
			counter++;
		}
		while (counter<8);
		counter=0;
	}
}
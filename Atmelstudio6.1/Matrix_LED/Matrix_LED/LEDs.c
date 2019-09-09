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
//声明4个数组临时变量
byte part_A[8],part_B[8],part_C[8],part_D[8],part_A_A[8],part_B_B[8],
part_C_C[8],part_D_D[8],part_E[8],part_F[8],part_G[8],part_H[8];

//字符列数据，规则是二维数组的第1维代表上下两个矩阵，第二维中第0数组和第1数组分别是左边的8*8点阵和右边的8*8点阵

byte Col[][2][16]={
	{
		{0xFD,0xFF,0xFD,0xFF,0xFD,0xFF,0xFC,0x03,0xFD,0xFF,0xFD,0xFF,0xC0,0x0F,0xDF,0xEF},
		{0xDF,0xEF,0xDF,0xEF,0xC0,0x0F,0xFF,0xFF,0xDB,0x77,0xDD,0xBB,0xBD,0xBB,0x7F,0xFB},/*"点",0*/
	}
	,
	{
		{0xFF,0xBF,0x83,0xBF,0xBB,0xBF,0xB4,0x01,0xB7,0x7F,0xAF,0x5F,0xB6,0xDF,0xB6,0x03},
		{0xBB,0xDF,0xBB,0xDF,0xBB,0xDF,0x94,0x01,0xAF,0xDF,0xBF,0xDF,0xBF,0xDF,0xBF,0xDF},/*"阵",1*/
	}
	,
	{
		{0xFF,0x7F,0xDF,0xBF,0xE8,0x03,0xEE,0xEF,0x7D,0xB7,0xBB,0x7B,0xBE,0xDF,0xEC,0x0F},
		{0xEF,0xEF,0xDF,0x77,0x1E,0xAF,0xDC,0xDF,0xDA,0xEF,0xD6,0xB7,0xDE,0x79,0xFE,0xFF},/*"滚",2*/
	}
	,
	{
		{0xFF,0xFF,0xC0,0x07,0xDF,0xF7,0xDF,0xF7,0xC0,0x07,0xDB,0xEF,0xDD,0xDF,0xD0,0x07},
		{0xDD,0xDF,0xDD,0xDF,0xC0,0x03,0xDD,0xDF,0xBD,0xDF,0xBB,0xDF,0x7B,0xDF,0xF7,0xDF},/*"屏",3*/
	}
	,
	{
		{0xDF,0xBF,0xC1,0x81,0xB7,0x6F,0x7F,0xFF,0xC0,0x07,0xDF,0xF7,0xC0,0x07,0xDF,0xF7},
		{0xC0,0x07,0xDF,0xF7,0xC0,0x07,0xF7,0xDF,0x00,0x01,0xF7,0xDF,0xEF,0xDF,0xDF,0xDF},/*"算",4*/
	}
	,
	{
		{0xFF,0xBF,0xDF,0xBF,0xEF,0xBF,0xEF,0xBF,0x78,0x03,0xBF,0xBF,0xBF,0xBF,0xEF,0xBF},
		{0xE0,0x01,0xDF,0xBF,0x1F,0x7F,0xDE,0xFF,0xDD,0xEF,0xDB,0xF7,0xD0,0x03,0xFB,0xFB},/*"法",5*/
	}
	,
};
byte Row[] = {1,2,4,8,16,32,64,128};
	
void setup() {
	pinMode(SH_CP, OUTPUT);
	pinMode(DS,OUTPUT);
	pinMode(ST_CP,OUTPUT);
}

//定义待显示字符数量
byte ch_num=6;

//定义显示速度变量
byte speed = 10;

void loop() {
	//初始化点阵移动计数器
	byte counter=0;
	//col[][]二维数组中两个数组，均为16个数据，循环16次，将数据分配各临时变量，4个临时变量对应4个8*8点阵，
	//对应顺序是part_A对应左上；part_B对应右上；part_C对应左下；
	//A和C点阵都是数组中的计数数据；B和C都是数组中的偶数数据
	for(int c=0; c<ch_num; c++){
		byte a=0;
		byte b=0;
		for(int i=0; i<16; i++){
			//i%2==0证明是数组中的偶数数据，包括0%2==0，故col数组中0,2,4,6,8,10,12,14,16相应数据装入part_A和part_C
			if(i%2==0)
			{
				//如果读取的字符数据时字符数据库col中的倒数第二个数据时，
				if(c==(ch_num-2)){
					part_A[a]=Col[c][0][i];
					part_C[a]=Col[c][1][i];
					//第二个点阵数据为倒数第一个数据
					part_A_A[a]=Col[c+1][0][i];
					part_C_C[a]=Col[c+1][1][i];
					//缓冲数据为字符库数据中开头数据
					part_E[a]=Col[0][0][i];
					part_G[a]=Col[0][1][i];
					//将col[]数组中的数据逐一分配给临时变量
					a++;
				}
				//如果读取的字符数据时字符数据库col中的倒数第一个数据时，
				else if(c==(ch_num-1)){
					part_A[a]=Col[c][0][i];
					part_C[a]=Col[c][1][i];
					//循环到字符数据库的开头数据
					part_A_A[a]=Col[0][0][i];
					part_C_C[a]=Col[0][1][i];
					part_E[a]=Col[1][0][i];
					part_G[a]=Col[1][1][i];
					//将col[]数组中的数据逐一分配给临时变量
					a++;
				}
				else
				//这段是正常思路：逐个获取字符数据
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
			//与i%2==0相对应，如果i%2≠0，就执行下面else里面的内容
			else
			{
				//如果读取的字符数据时字符数据库col中的倒数第二个字符数据时
				if(c==(ch_num-2)){
					part_B[b]=Col[c][0][i];
					part_D[b]=Col[c][1][i];
					//第二个点阵数据为字符数据库中最后一个字符数据
					part_B_B[b]=Col[c+1][0][i];
					part_D_D[b]=Col[c+1][1][i];
					//为了循环显示，缓冲数据回到字符数据库开头数据
					part_F[b]=Col[0][0][i];
					part_H[b]=Col[0][1][i];
					//将col[]数组中的数据逐一分配给临时变量
					b++;
				}
				//如果读取的字符数据时变量c为字符数据库col中的倒数第1个字符数据时
				else if(c==(ch_num-1)){
					part_B[b]=Col[c][0][i];
					part_D[b]=Col[c][1][i];
					part_B_B[b]=Col[0][0][i];
					part_D_D[b]=Col[0][1][i];
					part_F[b]=Col[1][0][i];
					part_H[b]=Col[1][1][i];
					b++;
				}
				//这个else里面的内容为正常思路，逐字符获取数据
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
		//第一次dowhile循环将点阵B数据移动到点阵A的位置；点着A_A数据移动到点阵B的位置。
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
		//第二次dowhile循环将点阵A_A数据移动到点阵A的位置；将点阵B_B的数据移动到点阵B的位置；第二个完整的字进入显示屏。
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
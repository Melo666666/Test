#include "bspPublic.h"
#include "string.h"



// 异或校验
u8 CheckXOR8(u8 *buf, u16 len)
{
	u8 i, check=0;
	
	for(i=0; i<len; i++)
	{
		check ^= buf[i];
	}
	return check;
}


//u32 str2Hex(char *str, char len)
//{
//	u32 val=0;
//	
//	for(u8 i=0; i<len; i++)
//	{
//		if(str[i] <= '9' && str[i] >= '0')
//		{
//			val = val*10 + str[i]-'0';
//		}
//	}
//	return val;
//}
// 字符串转HEX，单字节，strLen--字符串长度，1，2，3；base--进制，十进制写10，十六进制写16;
u8 str2Hex(char *str, u8 strLen, u8 base)
{
	u8 i,j, temp=0, value=0;
	for(i=0; i<strLen; i++)
	{
		if(base == 10){
			temp = str[i]-'0';
		}
		else if(base == 16)
		{
			if(str[i] >= 'A' && str[i] <= 'F')
				temp = str[i]-'A'+10;
			else if(str[i] >= '0' && str[i] <= '9')
				temp = str[i]-'0';
		}
		for(j=0; j<strLen-1-i; j++)
			temp *= base;
		value += temp;
	}
	return value;
}



void str2HexLong(char *str, u8 *convBuf)
{
	char i, *ptr=NULL;
	for(i=0; i<3; i++)
	{
		ptr = strstr(str, ".");
		if(ptr)
		{
			convBuf[i] = str2Hex(str, ptr-str, 10);
			ptr += 1;
			str = ptr;
		}
	}
	convBuf[i] = str2Hex(str, strlen(str), 10);
}


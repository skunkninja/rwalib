#include <string.h>
#include "rw_WinMacBasic.h"

int StringRadixChk(int radix,const wchar_t *source,wchar_t *Filtrate,bool CheckTail = false);
//Gatio
//通用字符串转数值
//参数1:字符串
//参数2:目标数值指针
//参数3:基数,表明为几进制,2,3,4,5,6,7,8,9,10,16,如果为0则为自动识别
//返回值:0-成功
//       1-基数错误
//       2-字符串不合要求
//       3-无法自动识别
int Gatoi(const wchar_t *string,int *data,int radix)
{
	wchar_t *S;
	int i;
	i=wcslen(string);
	if(i==0)
		return 2;

	S=new wchar_t [i+1];

	if(radix==0)//-进行自动识别字符串只识别2,10,16
	{
		//自动识别时必须有标识
		//如"11b"既可以理解为不加标识的16进制(0x11b),也可以理解为加标识的二进制(0x3)
		//在自动识别时认为是二进制
		if(StringRadixChk(2,string,S,true)==0)//是否2进制
		{
			radix=2;
		}
		else if(StringRadixChk(10,string,S,true)==0)//是否10进制
		{
			radix=10;
		}
		else if(StringRadixChk(16,string,S,true)==0)//是否16进制
		{
			radix=16;
		}
		else
		{
			i=3;
			goto GatoiExit;
		}
	}
	else if(radix<=10&&radix>1)
	{
		if(StringRadixChk(radix,string,S)!=0)
		{
			i=2;
			goto GatoiExit;
		}
	}
	else if(radix==16)
	{
		if(StringRadixChk(radix,string,S)!=0)
		{
			i=2;
			goto GatoiExit;
		}
	}
	else
	{
		i=1;
		goto GatoiExit;
	}

	*data=0;
	i=0;
	while(S[i]!='\0')
	{
		(*data)*=radix;
		if(S[i]<='9')
		{
			(*data)+=(S[i]-'0');
		}
		else
		{
			(*data)+=(S[i]-'A'+10);
		}
		i++;
	}

	i=0;
GatoiExit:
	delete [] S;
	return i;

}
//-检测字符串是否符合某一进制的要求
//参数1:进制-2,3,4,5,6,7,8,9,10,16
//参数2:源字符串
//参数3:过滤后的字符串
//参数4:是否必须判断尾标识
int StringRadixChk(int radix,const wchar_t *source,wchar_t *Filtrate,bool CheckTail)
{
	int i;
	int FiltrateStart,FiltrateEnd;
    wchar_t *pConvertSource = new wchar_t [wcslen(source)+1];

    rw_wcscpy(pConvertSource,wcslen(source)+1,source);
	for(i=0;pConvertSource[i]!='\0';i++)//-全部转成大写字母
	{
		if(pConvertSource[i]>='a'&&pConvertSource[i]<='z')
		{
			pConvertSource[i]-=('a'-'A');
		}
	}
	//这时i即为字符串的长度
	if(radix<=10)//检测10进制是否适合
	{
		//10进制字符串符合的条件为
		//最后一个字符可以为'D',其它都为'0'-'9'
		FiltrateEnd=i-1;
		if(pConvertSource[i-1]=='D')
		{
			if(radix==10)
				FiltrateEnd=i-2;
			else
            {
                delete [] pConvertSource;
				return 1;
            }
		}

		if(pConvertSource[i-1]=='B')
		{
			if(radix==2)
				FiltrateEnd=i-2;
			else
            {
                delete [] pConvertSource;
				return 1;
            }
		}

		//-如果在自动识别时没有头标识,没有尾标识又不是10进制,则错误
		if(CheckTail&&(FiltrateEnd==i-1)&&(radix!=10))
        {
            delete [] pConvertSource;
			return 1;
        }

		if(FiltrateEnd >= 0)////如果字符串为"d"或"b",就会导致FiltrateEnd<0
		{
			for(i=0;i<=FiltrateEnd;i++)
			{
				if(pConvertSource[i]<'0'||pConvertSource[i]>('0'+radix-1))
					break;
			}
			if(i<=FiltrateEnd)
			{
                delete [] pConvertSource;
				return 1;
			}
			else
			{
				FiltrateStart=0;
			}
		}
		else
		{
            delete [] pConvertSource;
			return 1;
		}
	}
	else if(radix==16)//检测16进制
	{
		FiltrateStart=0;
		FiltrateEnd=i-1;
		if((pConvertSource[0]=='0')&&(pConvertSource[1]=='X'))//0x打头的16进制
		{
			FiltrateStart=2;
		}

		if(pConvertSource[FiltrateEnd]=='H')//H结束的16进制
		{
			FiltrateEnd--;
		}

		if(CheckTail&&(pConvertSource[0]<'0'||pConvertSource[0]>'9'))//自动别时,字符串必须是数字开始(0x的0也是数字)
        {
            delete [] pConvertSource;
			return 1;
        }

		if(CheckTail&&(FiltrateStart==0)&&(FiltrateEnd==i-1))//自动识别时必须有16进制标识.
        {
            delete [] pConvertSource;
			return 1;
        }


		if(FiltrateEnd >= 0)//如果字符串为"h",就会导致FiltrateEnd<0
		{
			for(i=FiltrateStart;i<=FiltrateEnd;i++)
			{
				if((pConvertSource[i]>='0'&&pConvertSource[i]<='9')||(pConvertSource[i]>='A'&&pConvertSource[i]<='F'))
					continue;
				break;
			}
			if(i<=FiltrateEnd)
            {
                delete [] pConvertSource;
				return 1;
            }
		}
		else
		{
            delete [] pConvertSource;
			return 1;
		}
	}
	else
	{
        delete [] pConvertSource;
		return 1;
	}

	//--把字符过滤放到Filtrate中
	for(i=FiltrateStart;i<=FiltrateEnd;i++)
	{
		*Filtrate++=pConvertSource[i];
	}
	*Filtrate='\0';
    delete [] pConvertSource;
	return 0;
}

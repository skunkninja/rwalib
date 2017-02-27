#include "String2Words.h"
#include <string.h>

wchar_t defaultNotWordChars[] = {L",\t :"};

typedef struct
{
	int WordStart;//�����������ַ����еĿ�ʼ
	int WordLength;//���ʵĳ���
}_WORDDES;

wchar_t Comment[]={';'};//����ע�͵��ַ�,����ַ���������ַ����������Զ�����

CString2Words::CString2Words(void)
{
    pNotWordCharStr = defaultNotWordChars;

	WordsBuf = nullptr;
	iWordsSplit = 0;
	pWorkStringBuf = nullptr;
}

void CString2Words::FreeMemory()
{
	if(iWordsSplit > 0)
	{
		for(int i=0;i<iWordsSplit;i++)
		{
			if(WordsBuf!=nullptr&&WordsBuf[i]!=nullptr)
				delete [] WordsBuf[i];//Ҫ���ͷ��ַ����ڴ�
		}
		if(WordsBuf!=nullptr)
			delete [] WordsBuf;//���ͷŶ���ָ��.
		WordsBuf=nullptr;
	}
}

CString2Words::~CString2Words(void)
{
    FreeMemory();
    if(pWorkStringBuf != nullptr)
    {
        delete [] pWorkStringBuf;
        pWorkStringBuf = nullptr;
    }
}

void CString2Words::SetString(const wchar_t *line)
{
    int iLen = wcslen(line)+1;
    if(pWorkStringBuf != nullptr)
    {
        delete [] pWorkStringBuf;
        pWorkStringBuf = nullptr;
    }
    pWorkStringBuf = new wchar_t [iLen];
	wcscpy_s(pWorkStringBuf,iLen,line);
}
int CString2Words::SplitWords(int iWords2Split)
{
	if(pWorkStringBuf == nullptr)
	{
		return STR_WORDS_MEM_ERROR;
	}
    FreeMemory();

	iWordsSplit = StringWords(pWorkStringBuf,iWords2Split,&WordsBuf);
	return iWordsSplit;
}


int CString2Words::StringWords(const wchar_t *S,int Words,wchar_t ***OutBuf)
{
	int TotalWords;
	int index;
	int i;
	_WORDDES *WordDeses = nullptr;

	//��������
	i=0;//-Not In word
	bool bInQuote = false;
	TotalWords=0;
	for(index=0;;index++)
	{
		if(wcschr(Comment,int(S[index])))
			break;
		if(S[index] == '"')
		{
			bInQuote = !bInQuote;
			if(bInQuote)
			{
				if(i == 1)//start in another word. not correct.
				{
					return -1;
				}
				TotalWords++;
			}
			else
			{
			}
			continue;
		}
		if(bInQuote)
		{
			continue;
		}
		if(bInWord(S[index]))
		{
			if(i==0)
			{
				i=1;
				TotalWords++;
			}
		}
		else
		{
			i=0;
			if(S[index]=='\0')
			{
				break;
			}
		}
	}
	//--�����ʽ���
//	plog->AddFormatString("TotalWords:%d\n",TotalWords);
	if(Words==0)
		return TotalWords;//���Ҫ�����ĵ�����Ϊ��,�򷵻ص�����

	if(TotalWords == 0)
		return 0;
	//--���뵥����
	if(TotalWords<Words)//-����������ĵ��ʴ����ܵ�����,�������еĵ���
	{
		Words=TotalWords;//
	}

	WordDeses=new _WORDDES[Words];//�������ʵ�����

	if(WordDeses==nullptr)
		return STR_WORDS_MEM_ERROR;//�����ڴ����
	//��ȡ��������
	i=0;//-Not In word
	bInQuote = false;
	TotalWords=0;
	for(index=0;;index++)
	{
		if(S[index] == '"')
		{
			bInQuote = !bInQuote;
			if(bInQuote)
			{
				WordDeses[TotalWords].WordStart=index+1;
				TotalWords++;
			}
			else
			{
				WordDeses[TotalWords-1].WordLength=index-WordDeses[TotalWords-1].WordStart;
			}
			continue;
		}
		if(bInQuote)
		{
			continue;
		}

		if(bInWord(S[index]))
		{
			if(i==0)
			{
				i=1;
				WordDeses[TotalWords].WordStart=index;
				TotalWords++;
			}
		}
		else
		{
			if(i!=0)
			{
				WordDeses[TotalWords-1].WordLength=index-WordDeses[TotalWords-1].WordStart;
			}
			i=0;

			if(TotalWords>=Words)
				break;

			if(S[index]=='\0'||wcschr(Comment,int(S[index])))
				break;
		}
	}
	//--��ȡ������������

	if((*OutBuf)==nullptr)//����ڴ�û�з���,������ڴ�-��ע����ʹ����ɺ��ͷ�!!!
	{
		(*OutBuf)=new wchar_t * [Words];//����ָ������
		if(*OutBuf==nullptr)
		{
			delete [] WordDeses;
			return STR_WORDS_MEM_ERROR;
		}
		for(i=0;i<Words;i++)
		{
			(*OutBuf)[i]=new wchar_t [WordDeses[i].WordLength+1];//-�����ַ���������
			if((*OutBuf)[i]==nullptr)
			{
				for(Words=0;Words<i;Words++)
				{
					delete [] ((*OutBuf)[i]);//�ͷ������Ѿ��ͷŵ��ڴ�
				}
				delete [] *OutBuf;
				delete [] WordDeses;
				return STR_WORDS_MEM_ERROR;
			}
		}
	}

	//���ַ���������Ŀ�껺����
	for(i=0;i<Words;i++)
	{
		for(index=WordDeses[i].WordStart;index<(WordDeses[i].WordStart+WordDeses[i].WordLength);index++)
		{
			(*OutBuf)[i][index-WordDeses[i].WordStart]=S[index];
		}
		(*OutBuf)[i][index-WordDeses[i].WordStart]=0;
	}

	delete [] WordDeses;//-�ͷŵ����������ڴ�
	return Words;
}

const wchar_t *CString2Words::GetWord(int index)
{
	if(index < iWordsSplit)
	{
		return WordsBuf[index];
	}
	return nullptr;
}

bool CString2Words::bInWord(wchar_t testVal)
{
    if((testVal == '\0') || (testVal == '\r') || (testVal == '\n'))//
    {
        return false;
    }
    if(wcschr(pNotWordCharStr,testVal))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void CString2Words::SetNotInCharString(wchar_t *pNewNotInCharString)
{
    pNotWordCharStr = pNewNotInCharString;
}
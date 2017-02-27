#ifndef ARG_PROC_HEAD
#define ARG_PROC_HEAD

//����������:
//���ڴ��������� -arg:val �Ĳ���,ʹ��ʱ�������ִ����ݹ���,�������Զ����з��룬�жϡ�
/*ʾ��
CArgProc arg;
if(argc > 1) //��������,��������1
{
	for(i = 1;i < argc;i++)
	{
		arg.SetArgString(argv[i]);
		if(arg.IsArgument())//�ǲ���
		{
			if(_wcsicmp(arg.GetArgument(),_T("-w")) == 0)//�����ؼ���
			{
				if(arg.GetValue() != NULL)//�в���ֵ
				{
				}
			.......
}


/**/

typedef struct ARG_DEF
{
	wchar_t *strType;
	bool bIsArgument;
	bool bHasValue;
	wchar_t *strVal;
}ARG_DEFINE;

class CArgProc
{
protected:
	ARG_DEFINE workArg;
public:
	CArgProc(void);
	~CArgProc(void);

	void SetArgString(const wchar_t *pstr);
	bool IsArgument();
	wchar_t *GetArgument();
	wchar_t *GetValue();
};

#endif
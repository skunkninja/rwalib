#ifndef ARG_PROC_HEAD
#define ARG_PROC_HEAD

//参数处理类:
//用于处理类似于 -arg:val 的参数,使用时将参数字串传递过来,这个类会自动进行分离，判断。
/*示例
CArgProc arg;
if(argc > 1) //命令行下,参数大于1
{
	for(i = 1;i < argc;i++)
	{
		arg.SetArgString(argv[i]);
		if(arg.IsArgument())//是参数
		{
			if(_wcsicmp(arg.GetArgument(),_T("-w")) == 0)//参数关键字
			{
				if(arg.GetValue() != NULL)//有参数值
				{
				}
			.......
}
*/

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
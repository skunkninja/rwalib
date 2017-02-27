#ifndef STRING_WORDS_HEAD
#define STRING_WORDS_HEAD

#define STR_WORDS_MEM_ERROR -1

class CString2Words
{
public:
	CString2Words(void);
	~CString2Words(void);
	//Set string to process.
	void SetString(const wchar_t *line);
	//Split words.
	//iWords2Split is words number to process
	//Must > 0, return value should <=iWords2Split
	int SplitWords(int iWords2Split);
	
	//Get word by index.
	const wchar_t *GetWord(int index);
    //Set new string which contain all chars should not exist in a word.
    void SetNotInCharString(wchar_t *pNewNotInCharString);
protected:
	int StringWords(const wchar_t *S,int Words,wchar_t ***OutBuf);
    bool bInWord(wchar_t testVal);
    void FreeMemory();
	wchar_t *pWorkStringBuf;
	wchar_t **WordsBuf;
    const wchar_t *pNotWordCharStr;
	int iWordsSplit;
};

#endif
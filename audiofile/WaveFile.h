#ifndef WAVE_HEAD
#define WAVE_HEAD

//Wave文件类,未完成.
//现在只能进行重采样和保存文件.

///////////////////////////
//Wave结构
typedef struct 
{
	char rID[4];//RIFF文件标志
	unsigned int rLen;//长度
}_RiffHead;

typedef struct 
{
	char type[4];//RIFF文件类型
	char fID[4];//WAVE头标志
	unsigned int fLen;//WAVE头长度
	unsigned short wTag;//WAVE文件类型,01为PCM格式
	unsigned short wChannel;//通道数,立体声就是双通道
	unsigned int nSample;//取样频率
	unsigned int nByte;//一秒钟需要多少字节
	unsigned short align;//一个采样需要多少字节
	unsigned short sample;//位数
}_WaveChunk;

typedef struct
{
	char dID[4];
	unsigned int dSize;
}_WaveData;

typedef struct
{
	_RiffHead RiffHead;
	_WaveChunk WaveChunk;

}_WaveHead;

typedef struct
{
//	char sID[4];
//	unsigned int iSize;
	unsigned int iManufacturer;
	unsigned int iProduct;
	unsigned int iSamplePeriod;
	unsigned int iMidiNote;
	unsigned int iMidiPitchFraction;
	unsigned int iSampleFormat;
	unsigned int iSampleOffset;
	unsigned int iNumSampleLoops;
	unsigned int iSamplerData;
}_SampleChunk;

typedef struct
{
	unsigned int iID;
	unsigned int iLoopType;
	unsigned int iStart;
	unsigned int iEnd;
	unsigned int iFraction;
	unsigned int iPlayCount;
}_SampleLoopFormat;

class CWaveFile
{
public:
	CWaveFile();
	~CWaveFile(void);
	//void Resample(int iRate);
	//载入一个wave文件
	int LoadWave(wchar_t *strWaveName);
	//重采样wave文件,采用简单的线性重采样。
	//fRate是采样所使用的步进数
	//举例说明,如果原始采样是44.1k,现在需要采样为22.05K,则步进数需要置为2,即fRate=2.0
	bool Resample(float fRate);//resample wave file use float rate.
	//重采样,只是使用DSP中所使用的数据表示方法
	bool Resample(unsigned int iDspRate);//resample wave file use dsp rate.( 0xII FFFF)
	//保存wave文件
	int SaveWave(wchar_t *strWaveName);
	//获取采样率
	int GetSampleRate();

	//functions for create a new wave file
	//set how many Frames of new wave file
	void setWaveInfo(int totalTracks, int frameNumber, int sampleRate, int sampleBits);
	void CopyToNewFile(CWaveFile &newFile, int startMSTime, int endMSTime);
	void setTrackData(int *pData, int track);

	const wchar_t* GetWaveTypeString();
protected:
	wchar_t waveTypeString[30];
	_SampleChunk *pSampleChunk;
	_SampleLoopFormat *pSampleLoop;
	_WaveHead localWaveHead;

	int **pAllSampleData;//pointer to all sample data.
	int iFrameNumber;//frame num(sample number/ div channels). if stereo 16bit, 4bytes data, iSampleNum = 1.
	//释放所有的内存。
	void FreeMemory();
};

#endif
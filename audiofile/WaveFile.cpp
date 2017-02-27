#include "WaveFile.h"
#include "stdio.h"
#include <Windows.h>

CWaveFile::CWaveFile()
{
	pSampleChunk = NULL;
	pSampleLoop = NULL;
	localWaveHead.WaveChunk.wChannel = 0;
	pAllSampleData = NULL;
	iFrameNumber = 0;
//	LoadWave(strWaveName);
	ZeroMemory(&localWaveHead, sizeof(_WaveHead));
	memcpy(localWaveHead.RiffHead.rID, "RIFF", 4);

	memcpy(localWaveHead.WaveChunk.type, "WAVE", 4);
	memcpy(localWaveHead.WaveChunk.fID, "fmt ", 4);
	localWaveHead.WaveChunk.fLen = 16;
	localWaveHead.WaveChunk.wTag = 1;
}


CWaveFile::~CWaveFile(void)
{
	FreeMemory();
}

void CWaveFile::FreeMemory()
{
	int i;
	if(pSampleChunk != NULL)
	{
		delete pSampleChunk;
		pSampleChunk = NULL;
	}
	if(pSampleLoop != NULL)
	{
		delete pSampleLoop;
		pSampleLoop = NULL;
	}
	if(pAllSampleData != NULL)
	{
		for(i = 0;i < localWaveHead.WaveChunk.wChannel;i++)
		{
			if(pAllSampleData[i] != NULL)
			{
				delete pAllSampleData[i];
				pAllSampleData[i] = NULL;
			}
		}
		delete []pAllSampleData;
		pAllSampleData = NULL;
	}
}

int CWaveFile::LoadWave(wchar_t *strWaveName)
{
	int i,j;

	FILE *fpb;

	FreeMemory();

	fpb = NULL;
	errno_t err;
	err = _wfopen_s(&fpb,strWaveName,L"rb");
	if(err != 0)
	{
		return 1;//file was not opened
	}

	fseek(fpb,0,SEEK_END);
	int iFileSize = ftell(fpb);
	fseek(fpb,0,SEEK_SET);

	_WaveData WaveData;

	if(iFileSize > sizeof(_WaveHead))
	{
		fread(&localWaveHead,sizeof(_WaveHead),1,fpb);
		if(!memcmp(localWaveHead.RiffHead.rID,"RIFF",4)
			&&!memcmp(localWaveHead.WaveChunk.type,"WAVE",4)
			&&!memcmp(localWaveHead.WaveChunk.fID,"fmt ",4))
		{
			if (localWaveHead.WaveChunk.wChannel < 3)//only support mono or stereo.
			{
				fseek(fpb, localWaveHead.WaveChunk.fLen - 0x10, SEEK_CUR);
				while (1)
				{
					if (fread(&WaveData, sizeof(WaveData), 1, fpb) == 0)
					{
						break;
					}
					if (memcmp(WaveData.dID, "data", 4) == 0)
					{
						BYTE *pFileData;
						//SampleLen=WaveData.dSize;//bytes
						//pWork->iSampleBits = WaveHead.WaveChunk.sample;
						//pWork->iSampleChannels = WaveHead.WaveChunk.wChannel;
						//pWork->iSampleRate = WaveHead.WaveChunk.nSample;

						iFrameNumber = WaveData.dSize;//total bytes
						int iBytesPerSample = localWaveHead.WaveChunk.sample / 8;
						iFrameNumber /= localWaveHead.WaveChunk.wChannel;// div channels.
						iFrameNumber /= iBytesPerSample;//div bytes/sample
						pFileData = new unsigned char[WaveData.dSize];
						fread(pFileData, WaveData.dSize, 1, fpb);

						pAllSampleData = new int*[localWaveHead.WaveChunk.wChannel];
						for (i = 0; i < localWaveHead.WaveChunk.wChannel; i++)
						{
							pAllSampleData[i] = new int[iFrameNumber];//malloc track memory.
						}

						//------------------------------
						//copy data from Wave data to every track memory.
						int iWaveDataIndex = 0;
						int iTargetIndex = 0;
						for (i = 0; i < iFrameNumber; i++)
						{
							for (j = 0; j < localWaveHead.WaveChunk.wChannel; j++)
							{
								//--remember:8bit sample is unsigned BYTE
								*(pAllSampleData[j] + iTargetIndex) = 0;
								//modify the MSB/LSB data here.
								if (iBytesPerSample == 1)
								{
									memcpy(pAllSampleData[j] + iTargetIndex, pFileData + iWaveDataIndex, iBytesPerSample);
								}
								else
								{
									BYTE tmpBuffer[4];
									memcpy(tmpBuffer, pFileData + iWaveDataIndex, iBytesPerSample);

									if (localWaveHead.WaveChunk.wTag == 1) //windows PCM, LSB
									{
										memcpy(pAllSampleData[j] + iTargetIndex, tmpBuffer, iBytesPerSample);
										wcscpy_s(waveTypeString, L"Windows PCM(LSB)");
									}
									else if (localWaveHead.WaveChunk.wTag == 0xfe02) //MAC PCM, MSB
									{
										BYTE swapBuffer[4];

										for (int offset = 0; offset < iBytesPerSample; offset++)
										{
											swapBuffer[offset] = tmpBuffer[iBytesPerSample - 1 - offset];
										}
										memcpy(pAllSampleData[j] + iTargetIndex, swapBuffer, iBytesPerSample);

										wcscpy_s(waveTypeString, L"MAC PCM(MSB)");
									}
									else
									{
										return 4;
									}
								}
								

								iWaveDataIndex += iBytesPerSample;

								if (iBytesPerSample == 1)
								{
									//8bit changed unsigned to signed.
									*(pAllSampleData[j] + iTargetIndex) -= 0x80;
									*(pAllSampleData[j] + iTargetIndex) &= 0xff;

									*(pAllSampleData[j] + iTargetIndex) <<= 16;//use 24bit
								}
								else if (iBytesPerSample == 2)
								{
									*(pAllSampleData[j] + iTargetIndex) <<= 8;//use 24bit
								}

								if (*(pAllSampleData[j] + iTargetIndex) & 0x800000)
								{
									*(pAllSampleData[j] + iTargetIndex) |= 0xff000000;//neg value.
								}
								//more bytes/sample now not support correctly.
							}
							iTargetIndex++;
						}
						//copy all data to every track. use 24bit.
						delete[] pFileData;
					}
					else if (memcmp(WaveData.dID, "smpl", 4) == 0)
					{
						//i = 0;
						UCHAR *pSmplData;
						pSmplData = new unsigned char[WaveData.dSize];
						fread(pSmplData, WaveData.dSize, 1, fpb);

						pSampleChunk = new _SampleChunk;

						memcpy(pSampleChunk, pSmplData, sizeof(_SampleChunk));
						if (pSampleChunk->iNumSampleLoops != 0)
						{
							pSampleLoop = new _SampleLoopFormat;
							memcpy(pSampleLoop, pSmplData + sizeof(_SampleChunk), sizeof(_SampleLoopFormat));
						}

						delete[] pSmplData;
					}
					else
					{
						fseek(fpb, WaveData.dSize, SEEK_CUR);
					}
				}
				i = 0;
			}
			else
			{
				//printf("Not mono file!\n");
				i = 3;
			}
		}
		else
		{
			//printf("Not wave file!\n");
			i = 4;
		}
	}
	else
	{
		//printf("Not wave file!\n");
		i = 4;
	}

	localWaveHead.WaveChunk.wTag = 1;//force to Microsoft PCM format
	if(fpb != NULL)
	{
		fclose(fpb);
	}
	return i;
}

int CWaveFile::GetSampleRate()
{
	return localWaveHead.WaveChunk.nSample;
}

//resample wave file use float rate.
//return:
//  true - success
//  false - error.
bool CWaveFile::Resample(float fRate)
{
	int i,j;
	int iNewSampleNum;
	int **pReSampleData;

	if(pAllSampleData == NULL)
	{
		return false;//no data!
	}

	iNewSampleNum = int(iFrameNumber / fRate) + 1;

	pReSampleData = new int*[localWaveHead.WaveChunk.wChannel];
	for(i = 0;i < localWaveHead.WaveChunk.wChannel;i++)
	{
		pReSampleData[i] = new int[iNewSampleNum];
	}

	for(i = 0;i < localWaveHead.WaveChunk.wChannel;i++)
	{
		float fResamplePos = 0.0f;
		int iResamplePos;
		float fFracPos;
		for(j = 0;j<iNewSampleNum;j++)
		{
			fResamplePos = j*fRate;
			iResamplePos = int(fResamplePos);
			fFracPos = fResamplePos-float(iResamplePos);
			if (iResamplePos < iFrameNumber - 1)
			{
				*(pReSampleData[i]+j)=int(float(*(pAllSampleData[i]+iResamplePos))*fFracPos
					+float(*(pAllSampleData[i]+iResamplePos+1))*(1.0f-fFracPos));
			}
			else
			{
				break;
			}
		}

		iNewSampleNum = j;
	}

	for(i = 0;i < localWaveHead.WaveChunk.wChannel;i++)
	{
		if(pAllSampleData[i] != NULL)
		{
			delete pAllSampleData[i];
			pAllSampleData[i] = NULL;
		}
	}
	delete []pAllSampleData;
	
	pAllSampleData = pReSampleData;
	iFrameNumber = iNewSampleNum;

	return true;//
}
//resample wave file use dsp rate.( 0xII FFFF)
//return:
//  true - success
//  false - error.
bool CWaveFile::Resample(unsigned int iDspRate)
{
	float fRate;
	int iRate;
	iRate = iDspRate >> 16;
	fRate = (iDspRate&0xffff)/65536.f+float(iRate);
	return Resample(fRate);
}
//save wave file
int CWaveFile::SaveWave(wchar_t *strWaveName)
{
	int i,j;
	FILE *fpb;
	fpb = NULL;
	errno_t err;

	if ((localWaveHead.WaveChunk.wChannel == 0) || (iFrameNumber == 0))
	{
		return 2;
	}

	err = _wfopen_s(&fpb,strWaveName,L"wb");
	if(err != 0)
	{
		return 1;//file was not opened
	}

	_WaveData WaveData;

	memcpy(WaveData.dID,"data",4);
	WaveData.dSize = iFrameNumber*localWaveHead.WaveChunk.wChannel*(localWaveHead.WaveChunk.sample / 8);
    //When read wave file, the length can be any value, but fixed to 0x10 when save.
    //all additional informatin will be lost.
    localWaveHead.WaveChunk.fLen = 0x10;
	localWaveHead.RiffHead.rLen = WaveData.dSize + sizeof(_WaveChunk)+8;

	fwrite(&localWaveHead,sizeof(_WaveHead),1,fpb);
	fwrite(&WaveData,sizeof(_WaveData),1,fpb);

	int iTmpSampleData;

	for (i = 0; i < iFrameNumber; i++)
	{
		for(j = 0;j < localWaveHead.WaveChunk.wChannel;j++)
		{
			iTmpSampleData = *(pAllSampleData[j]+i);
			iTmpSampleData >>= (24-localWaveHead.WaveChunk.sample);
			if(localWaveHead.WaveChunk.sample == 8)
			{
				iTmpSampleData += 0x80;
			}
			fwrite(&iTmpSampleData,localWaveHead.WaveChunk.sample/8,1,fpb);
		}
	}

	fclose(fpb);
	return 0;
}

void CWaveFile::CopyToNewFile(CWaveFile &newFile, int startMSTime, int endMSTime)
{
	int startFrame;
	int endFrame;
	__int64 tmpStartMSTime = startMSTime;
	__int64 tmpEndMSTime = endMSTime;

	startFrame = tmpStartMSTime*localWaveHead.WaveChunk.nSample / 1000;
	endFrame = tmpEndMSTime*localWaveHead.WaveChunk.nSample / 1000;
	if (startFrame >= iFrameNumber)
	{
		startFrame = iFrameNumber - 1;
	}

	if (endFrame >= iFrameNumber)
	{
		endFrame = iFrameNumber - 1;
	}

	if (startFrame >= endFrame)
	{
		return;
	}
	newFile.setWaveInfo(localWaveHead.WaveChunk.wChannel, endFrame - startFrame, localWaveHead.WaveChunk.nSample, localWaveHead.WaveChunk.sample);

	for (int j = 0; j < localWaveHead.WaveChunk.wChannel; j++)
	{
		newFile.setTrackData(pAllSampleData[j]+startFrame, j);
	}
}

void CWaveFile::setWaveInfo(int totalChannels, int frameNumber, int sampleRate, int sampleBits)
{
	FreeMemory();

	iFrameNumber = frameNumber;
	localWaveHead.WaveChunk.align = sampleBits*totalChannels / 8;
	localWaveHead.WaveChunk.nByte = localWaveHead.WaveChunk.align*sampleRate;
	localWaveHead.WaveChunk.nSample = sampleRate;
	localWaveHead.WaveChunk.sample = sampleBits;
	localWaveHead.WaveChunk.wChannel = totalChannels;

	pAllSampleData = new int*[localWaveHead.WaveChunk.wChannel];
	for (int i = 0; i < localWaveHead.WaveChunk.wChannel; i++)
	{
		pAllSampleData[i] = new int[iFrameNumber];//malloc track memory.
	}
}

void CWaveFile::setTrackData(int *pData, int track)
{
	memcpy(pAllSampleData[track], pData, iFrameNumber*sizeof(int));
}

const wchar_t* CWaveFile::GetWaveTypeString()
{
	return waveTypeString;
}
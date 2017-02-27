#ifndef JUCE_START_FILE_BASIC_H
#define JUCE_START_FILE_BASIC_H

#include "JuceHeader.h"
//一些对文件名的操作函数,都很简单。

//给定一个文件名，得到其所在路径。
//如果给的是FullPath，即文件名中包含路径，则直接获取路径
//如果给的是相对路径，则在当前工作路径上进行处理。
//输入:
//    filename - 文件名称
//输出:
//    filename - 文件的路径
void FileNameToPath(String *filename);

//路径转换由以前的工作路径和文件名，得到现在的文件带路径名称。
//比如说filename = "c:/test/aa.txt"
//      savepath = "c:/test"
//      curentPath = "d:/test2"
//则返回filename = "d:/test2/aa.txt"
//主要用于需要使用相对路径的场合。
void ConvertFilePath(String *filename,String savepath,String currentPath);

//给文件名加上后缀
//比如说SourceFileName = "c:\test\aa.wav"
//      nameToAdd = "_2X"
//则返回SourceFileName = "c:\test\aa_2X.wav"
void FileNameAddStr(String *sourceFileName,String nameToAdd);
#endif
#ifndef JUCE_START_FILE_BASIC_H
#define JUCE_START_FILE_BASIC_H

#include "JuceHeader.h"

//Function for file names
//Only work with Juce.

//Get file path:
//If "\" or "/" is included in give file name, just return string without file name
//else return current work path.
//Input:
//    filename - File name
//Output:
//    filename - File path ("/" included in file path)
void FileNameToPath(String *filename);

//Convert full file name with path to new file name with path.
//This function is useful when Relative Path needed in the program:
//For example:
//filename = "c:/test/aa.txt"  (work file name saved in file)
//      savepath = "c:/test"   (work path saved in file)
//      curentPath = "d:/test2"  (current work path)
//return filename = "d:/test2/aa.txt"  (get current work file which use relative path)
void ConvertFilePath(String *filename,String savepath,String currentPath);

//Add a suffix to file name
//For example
//    SourceFileName = "c:\test\aa.wav"
//      nameToAdd = "_2X"
//return
//    SourceFileName = "c:\test\aa_2X.wav"
void FileNameAddStr(String *sourceFileName,String nameToAdd);

//Mix newPath to new Path
//For example:
//1.newPath = "\aa"  (absolute path 1)
//  What ever the sourcePath is, target is "\aa"
//2.newPath = "d:\aa"  (absolute path 2)
//  What ever the sourcePath is, target is "d:\aa"
//3.newPath = "bb\cc"
//  sourcePath = "d:\aa"
//  targetPath = "d:\aa\bb\cc"
//4.newPath = "..\bb"
//  sourcePath = "d:\aa"
//  targetPath = "d:\aa\..\bb"  --> Windows will convert the path to "d:\bb", I didn't do the convert now.
void MixFilePath(wchar_t *target,wchar_t *sourcePath,const wchar_t *newPath);

//  convert file path
//For example:
//   C:/aa/bb/cc/../../..  -> c:/
//   C:/aa/./bb/./cc       -> c:/aa/bb/cc
//   aa/bb/cc/../..        -> aa/
//   
//error: will keep no change.
//   c:/..        error
//   aa/../..     error
void FormatPath(wchar_t *path);
#endif
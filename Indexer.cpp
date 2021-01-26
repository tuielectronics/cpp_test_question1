#include "stdafx.h"
#include "Indexer.h"
#include "stdlib.h"
#include "stdio.h"
#include "io.h"
#include "string.h"

#include <iostream>
#include <typeinfo>  
void Indexer::initIndexes()
{
	insertIndex = 0;
	maxIndex = 1024;
	fileIndex = new FileDef*[1024];
	memset(fileIndex, 0, maxIndex);
}
void Indexer::processDirectory(const char* name) {
	if (init) {
		init = false;
		initIndexes();
	}

	char search[256];
	strcpy(search, name);
	strcat(search, "\\*.*");

	_finddata_t fd;
	intptr_t h = _findfirst(search, &fd);
	if (h != -1 && strlen(name) + strlen(fd.name) + 1 < 256) // prevent strcpy/strcat overflow 
	{
		do {
			FileDef* f = new FileDef;
			f->size = fd.size;

			char fname[256];
			strcpy(fname, name);
			strcat(fname, "\\");
			strcat(fname, fd.name);
			_splitpath(fname, f->drive, f->path, f->name, f->ext);
			if (strcmp(f->ext, ".txt") == 0) {
				f->type = txt;
			}
			else if (strcmp(f->ext, ".xml") == 0) {
				f->type = xml;
			}
			else if (strcmp(f->ext, ".exe") == 0) {
				f->type = exe;
			}
			else if (strcmp(f->ext, ".doc") == 0) {
				f->type = doc;
			}
			else if (strcmp(f->ext, ".xls") == 0) {
				f->type = xls;
			}
			else if (strcmp(f->ext, ".ppt") == 0) {
				f->type = ppt;
			}
			else {
				f->type = skip;
			}
			if (f->type != skip) {
				fileIndex[insertIndex++] = f;
			}

		} while (insertIndex < maxIndex && _findnext(h, &fd) == 0);
		_findclose(h);
	}


}

int Indexer::getFirstFile(FileType type, FileDef* fd) //change return type from int* to int
{
	int tmp = 0;
	while (tmp < maxIndex && fileIndex[tmp] != 0 && fileIndex[tmp]->type != type) ++tmp;//add maxIndex check
	int result = -1;
	if ((result = (fileIndex[tmp] == 0) ? -1 : tmp + 1) == -1)
		return result;
	*fd = *fileIndex[tmp];
	
	return result;
}

int Indexer::getNextFile(int* handle, FileDef* fd)
{
	int tmp = *handle;
	while (tmp < maxIndex && fileIndex[tmp] != 0 && fileIndex[tmp]->type != fd->type) ++tmp;//add maxIndex check
	if (fileIndex[tmp] != 0) {
		*fd = *fileIndex[tmp];
		*handle = tmp + 1;
		return 0;
	}
	return -1;
}

void Indexer::listFiles(FileType type)
{
	printf("%30s%7s\r\n", "NAME", "SIZE");
	FileDef fd;
	int handle = getFirstFile(type, &fd);
	if (handle != -1) {
		do {
			printf("%30s%7i\r\n", fd.name, fd.size);
		} while (getNextFile(&handle, &fd) == 0);
	}
}

bool Indexer::needsBackup(const FileDef* fd)
{
	switch (fd->type) {
	case doc:
	case xls:
		return true;
	}
	return false;
}

bool Indexer::needsScan(const FileDef* fd)
{
	return (fd->type == exe);
}

// test function
void Indexer::_testIndexer() {
	
	processDirectory("E:\\test1");
	
	processDirectory("E:\\test2");
	listFiles(exe);
	listFiles(txt);
	listFiles(xml);
	
}
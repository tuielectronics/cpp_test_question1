#ifndef INDEXER_H
#define INDEXER_H

enum FileType {
	skip = 0,
	txt = 1,
	xml = 2,
	exe = 3,
	doc = 4,
	xls = 5,
	ppt = 6,
};

struct FileDef {
	char		drive[4];
	char		name[256];
	char		path[256];
	char		ext[4];
	int			size;
	FileType	type;
};

class Indexer
{
private:
	FileDef** fileIndex;
	int insertIndex;
	int maxIndex;
	bool init = true;

public:
	void initIndexes();
	void processDirectory(const char* name);
	int getFirstFile(FileType type, FileDef* fd);
	int getNextFile(int* handle, FileDef* fd);
	void listFiles(FileType type);
	bool needsBackup(const FileDef* fd);
	bool needsScan(const FileDef* fd);
	void _testIndexer();
};
#endif //INDEXER_H


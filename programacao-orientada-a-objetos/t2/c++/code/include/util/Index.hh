#ifndef _INDEX_HH_
#define _INDEX_HH_

#define CHAR   0
#define CCHAR  1
#define INT    2
#define FLOAT  3
#define DOUBLE 4

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../util/Object.hh"
#include "../util/Vector.hh"
#include "../type/String.hh"

class IndexRecord : public Object {
	public:
		void *data;
		int offset;

		IndexRecord(void *data, int offset);
		virtual ~IndexRecord();
};

class Index : public Object {
	private:
		Vector<IndexRecord *> *records;
		int fieldType;
		int fieldSize;
		int *matches;
		int foundCount;

		void clear();
		void release();
		void sort();
		void mergesort(int start, int end);
	public:
		Index(int fieldType, int fieldSize);
		virtual ~Index();
		int match(int i);
		int found();
		void create(const char *fileDAT, int fileRecordSize, int fieldOffset);
		void load(const char *fileIDX);
		void make(const char *fileIDX);
		void binarySearch(String *key);
		void binarySearch(const char *key);
		void sequentialSearch(const char *key);
};

#endif

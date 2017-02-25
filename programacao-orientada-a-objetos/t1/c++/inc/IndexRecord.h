#ifndef _INDEXRECORD_H_
#define _INDEXRECORD_H_

#include <stdlib.h>

/**
 * Classe IndexRecord
 * Armazena um registro de um arquivo de índice
 */
class IndexRecord {
	public:
		void *data; // o dado
		int offset; // o offset

		/**
		 * Construtor
		 */
		IndexRecord(void *data, int offset) {
			this->data = data;
			this->offset = offset;
		}
		
		/**
		 * Destrutor
		 */
		~IndexRecord() {
			if (this->data != NULL) free(data);
		}
};

#endif

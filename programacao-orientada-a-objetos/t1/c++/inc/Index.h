#ifndef _INDEX_H_
#define _INDEX_H_

// Valores numéricos para os tipos de campos
#define CHAR   0 // caractere
#define CCHAR  1 // cadeia de caractereres
#define INT    2 // inteiro
#define FLOAT  3 // real
#define DOUBLE 4 // real com mais precisão

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Vector.h>
#include <IndexRecord.h>

/**
 * Classe Index
 */
class Index {
	private:
		Vector<IndexRecord *> *records;
		int fieldType;
		int fieldSize;
		int *matches;
		int foundCount;
		
		/**
		 * Zera os resultados da busca anterior
		 */
		void clear() {
			this->foundCount = 0;
			if (this->matches != NULL) {
				free(this->matches);
				this->matches = NULL;
			}
		}

		/**
		 * Descarrega os dados de records que foram carregados
		 */		
		void release() {
			for (int i = 0, size = this->records->size(); i < size; i++) {
				delete this->records->get(i);
			}
			this->records->clear();
		}
		
		/**
		 * Algoritmo de ordenação mergesort para ordenar os records
		 */
		void sort() {
			this->mergesort(0, this->records->size()-1);
		}

		void mergesort(int start, int end) {
			int central = (start + end) / 2;

			if (start >= end) return;

			this->mergesort(start, central);
			this->mergesort(central+1, end);
			{
				Vector<IndexRecord *> *right, *left;
				int i, j, k, rightSize, leftSize;

				rightSize = end - central;
				leftSize  = central - start + 1;

				right = new Vector<IndexRecord *>(rightSize);
				left = new Vector<IndexRecord *>(leftSize);

				this->records->copyInto(right, central+1);
				this->records->copyInto(left, start);
				
				i = j = 0;
				for (k = start; k < end+1; k++) {
					if (j < rightSize && i < leftSize) {

						// tipo INT
						if (this->fieldType == INT) {
							int leftValue = *((int *) left->get(i)->data);
							int rightValue  = *((int *) right->get(j)->data);

							if (leftValue > rightValue) {
								this->records->add(k, right->get(j));
								j++;
							} else {
								this->records->add(k, left->get(i));
								i++;
							}

						// tipo CHAR
						} else if (this->fieldType == CHAR) {
							char leftValue = *((char *) left->get(i)->data);
							char rightValue  = *((char *) right->get(j)->data);

							if (leftValue > rightValue) {
								this->records->add(k, right->get(j));
								j++;
							} else {
								this->records->add(k, left->get(i));
								i++;
							}

						// tipo CCHAR
						} else if (this->fieldType == CCHAR) {
							char *leftValue = (char *) calloc(1, this->fieldSize + sizeof(char));
							char *rightValue  = (char *) calloc(1, this->fieldSize + sizeof(char));
							
							strncpy(leftValue, (char *) left->get(i)->data, this->fieldSize);
							strncpy(rightValue, (char *) right->get(j)->data, this->fieldSize);

							if (strcmp(leftValue, rightValue) > 0) {
								this->records->add(k, right->get(j));
								j++;
							} else {
								this->records->add(k, left->get(i));
								i++;
							}
							
							free(leftValue);
							free(rightValue);

						// tipo FLOAT
						} else if (this->fieldType == FLOAT) {
							float leftValue = *((float *) left->get(i)->data);
							float rightValue  = *((float *) right->get(j)->data);

							if (leftValue > rightValue) {
								this->records->add(k, right->get(j));
								j++;
							} else {
								this->records->add(k, left->get(i));
								i++;
							}

						// tipo DOUBLE
						} else if (this->fieldType == DOUBLE) {
							double leftValue = *((double *) left->get(i)->data);
							double rightValue  = *((double *) right->get(j)->data);

							if (leftValue > rightValue) {
								this->records->add(k, right->get(j));
								j++;
							} else {
								this->records->add(k, left->get(i));
								i++;
							}
						}

					} else if (j < rightSize) {
						this->records->add(k, right->get(j));
						j++;
					} else if (i < leftSize) {
						this->records->add(k, left->get(i));
						i++;
					}
				}
				delete right;
				delete left;
			}
		}
		
	public:
		/**
		 * Retorna um match da posição i
		 */
		int match(int i) {
			if (this->matches != NULL && i >= 0 && i < this->foundCount)
				return this->matches[i];
			return -1;
		}
		
		/**
		 * Retorna a variável foundCount
		 */
		int found() {
			return this->foundCount;
		}
	
		/**
		 * Aloca records a partir de um arquivo .dat
		 */
		void create(const char *fileDAT, int fileRecordSize, int fieldOffset) {
			this->release();

			FILE *fpDAT = fopen(fileDAT, "rb");
			if (fpDAT != NULL) {
				int fileOffset = fieldOffset;
				void *data;

				fseek(fpDAT, fileOffset, SEEK_SET);
				while (!feof(fpDAT)) {
					data = calloc(1, this->fieldSize);
					fread(data, this->fieldSize, 1, fpDAT);
					if (!feof(fpDAT)) {
						IndexRecord *ir = new IndexRecord(data, fileOffset - fieldOffset);
						this->records->add(ir);
						fileOffset += fileRecordSize;
						fseek(fpDAT, fileOffset, SEEK_SET);
					} else
						free(data);
				}
				fclose(fpDAT);

				this->sort();
			}
		}
		
		/**
		 * Aloca os registros a partir de um arquivo .idx
		 */
		void load(const char *fileIDX) {
		 	this->release();

		 	FILE *fpIDX = fopen(fileIDX, "rb");
		 	if (fpIDX != NULL) {
			 	void *data;
			 	int offset;
			 	
				while (!feof(fpIDX)) {
			 		data = calloc(1, this->fieldSize);
					fread(data, this->fieldSize, 1, fpIDX);
					fread(&offset, sizeof(int), 1, fpIDX);
					if (!feof(fpIDX)) {
						IndexRecord *ir = new IndexRecord(data, offset);
						this->records->add(ir);
					} else
						free(data);
				}
			 	fclose(fpIDX);

				this->sort();
			}
		 }
	
		/**
		 * Construtor
		 */
		Index(int fieldType, int fieldSize) {
			this->records = new Vector<IndexRecord *>;
			this->fieldType = fieldType;
			this->fieldSize = fieldSize;
			this->matches = NULL;
			this->foundCount = 0;
		}
			
		/**
		 * Destrutor
		 */
		~Index() {
			if (this->matches != NULL) {
				free(this->matches);
			}
			for (int i = 0, size = this->records->size(); i < size; i++) {
				delete this->records->get(i);
			}
			delete this->records;
		}
		
		/**
		 * Cria o arquivo .idx
		 */
		void make(const char *fileIDX) {
			FILE *fpIDX = fopen(fileIDX, "wb+");
			if (fpIDX != NULL) {
				for (int i = 0, size = this->records->size(); i < size; i++) {
					fwrite(this->records->get(i)->data, this->fieldSize, 1, fpIDX);
					fwrite(&(this->records->get(i)->offset), sizeof(int), 1, fpIDX);
				}
				fclose(fpIDX);
			}
		}

		/**
		 * Busca binária iterativa em um arquivo .idx
		 */
		void binarySearch(const char *key) {
			this->clear();
			
			// Define start e end
			int start = 0, end = this->records->size()-1, central, at = -1;

			while (start <= end) {
				// Calcula o central
				central = (start + end) / 2;

				// tipo INT
				if (this->fieldType == INT) {
					int keyRead = *((int *) this->records->get(central)->data);
					int keyWanted = atoi(key);

					if (keyWanted == keyRead) {
						// Confere se existem chaves anteriores com o mesmo valor procurado
						at = central;
						if (central > 0) {
							at = central-1;
							do {
								keyRead = *((int *) this->records->get(at)->data);
								if (keyWanted == keyRead)
									at--;
								else
									break;
							} while (at >= 0);
							at++;
						}
						break;
					}
					keyWanted > keyRead ? start = central+1 : end = central-1;

				// tipo CHAR
				} else if (this->fieldType == CHAR) {
					char keyRead = *((char *) this->records->get(central)->data);
					char keyWanted = *key;

					if (keyWanted == keyRead) {
						// Confere se existem chaves anteriores com o mesmo valor procurado
						at = central;
						if (central > 0) {
							at = central-1;
							do {
								keyRead = *((char *) this->records->get(at)->data);	
								if (keyWanted == keyRead)
									at--;
								else
									break;
							} while (at >= 0);
							at++;
						}
						break;
					}
					keyWanted > keyRead ? start = central+1 : end = central-1;

				// tipo CCHAR
				} else if (this->fieldType == CCHAR) {
					char *keyRead = (char *) calloc(1, this->fieldSize + sizeof(char));
					const char *keyWanted = key;

					strncpy(keyRead, (char *) this->records->get(central)->data, this->fieldSize);

					if (strcmp(keyWanted, keyRead) == 0) {
						// Confere se existem chaves anteriores com o mesmo valor procurado
						at = central;
						if (central > 0) {
							at = central-1;
							do {
								strncpy(keyRead, (char *) this->records->get(at)->data, this->fieldSize);
								if (strcmp(keyWanted, keyRead) == 0)
									at--;
								else
									break;
							} while (at >= 0);
							at++;
						}
						free(keyRead);
						break;
					}
					strcmp(keyWanted, keyRead) > 0 ?  start = central+1 : end = central-1;
					free(keyRead);

				// tipo FLOAT
				} else if (this->fieldType == FLOAT) {
					float keyRead = *((float *) this->records->get(central)->data);
					float keyWanted = atof(key);

					if (keyWanted == keyRead) {
						// Confere se existem chaves anteriores com o mesmo valor procurado
						at = central;
						if (central > 0) {
							at = central-1;
							do {
								keyRead = *((float *) this->records->get(at)->data);
								if (keyWanted == keyRead)
									at--;
								else
									break;
							} while (at >= 0);
							at++;
						}
						break;
					}
					keyWanted > keyRead ? start = central+1 : end = central-1;

				// tipo DOUBLE
				} else if (this->fieldType == DOUBLE) {
					double keyRead = *((double *) this->records->get(central)->data);
					double keyWanted = atof(key);

					if (keyWanted == keyRead) {
						// Confere se existem chaves anteriores com o mesmo valor procurado
						at = central;
						if (central > 0) {
							at = central-1;
							do {
								keyRead = *((double *) this->records->get(at)->data);
								if (keyWanted == keyRead)
									at--;
								else
									break;
							} while (at >= 0);
							at++;
						}
						break;
					}
					keyWanted > keyRead ? start = central+1 : end = central-1;
				}
			}
			if (at != -1) {
				// Aloca em matches os offsets dos registros encontrados em ordem
				while (at <= central) {
					this->matches = (int *) realloc(this->matches, sizeof(int) * (this->foundCount + 1));
					this->matches[this->foundCount++] = this->records->get(at++)->offset;
				}
			}
		}

		/**
		 * Busca sequencial em um arquivo .idx
		 */
		void sequentialSearch(const char *key) {
			this->clear();
			
			bool flag = false;

			for (int i = 0, size = this->records->size(); i < size; i++) {
				flag = false;

				// tipo INT
				if (this->fieldType == INT) {
					int keyRead = *((int *) this->records->get(i)->data);
					int keyWanted = atoi(key);

					if (keyWanted == keyRead)
						flag = true;

				// tipo CHAR
				} else if (this->fieldType == CHAR) {
					char keyRead = *((char *) this->records->get(i)->data);
					char keyWanted = *key;

					if (keyWanted == keyRead)
						flag = true;

				// tipo CCHAR
				} else if (this->fieldType == CCHAR) {
					char *keyRead = (char *) calloc(1, this->fieldSize + sizeof(char));
					const char *keyWanted = key;

					strncpy(keyRead, (char *) this->records->get(i)->data, this->fieldSize);

					if (strcmp(keyWanted, keyRead) == 0)
						flag = true;
					
					free (keyRead);

				// tipo FLOAT ou DOUBLE
				} else if (this->fieldType == FLOAT) {
					float keyRead = *((float *) this->records->get(i)->data);
					float keyWanted = atof(key);

					if (keyWanted == keyRead)
						flag = true;

				// tipo DOUBLE
				} else if (this->fieldType == DOUBLE) {
					double keyRead = *((double *) this->records->get(i)->data);
					double keyWanted = atof(key);

					if (keyWanted == keyRead)
						flag = true;
				}
				
				if (flag) {
					// Aloca em matches os offsets dos registros encontrados em ordem
					this->matches = (int *) realloc(this->matches, sizeof(int) * (this->foundCount + 1));
					this->matches[this->foundCount++] = this->records->get(i)->offset;
				}
			}
		}

};

#endif

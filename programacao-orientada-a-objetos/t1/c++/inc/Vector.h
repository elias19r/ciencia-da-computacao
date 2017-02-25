#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>

/**
 * Classe Vector
 * Implementa um vetor genérico
 */
template <class T>
class Vector {
	private:
		int elementCount;
		T *elementData;

	public:	
		/**
		 * Construtores
		 */
		Vector() {
			this->elementCount = 0;
			this->elementData  = NULL;
		}

		Vector(int initialCapacity) {
			this->elementCount = 0;
			this->elementData  = NULL;
			this->ensureCapacity(initialCapacity);
		}

		/**
		 * Destrutor
		 */
		~Vector() {
			if (this->elementData != NULL) free(this->elementData);
		}
		
		/**
		 * Adiciona um elemento no vetor
		 */
		void add(T element) {
			this->elementData = (T *) realloc(this->elementData, sizeof(T) * (this->elementCount + 1));
			this->elementData[this->elementCount++] = element;
		}
		
		/**
		 * Adiciona um elemento na posição index do vetor
		 */
		bool add(int index, T element) {
			if (this->validIndex(index)) {
				this->elementData[index] = element;
				return true;
			} else {
				return false;
			}
		}
		
		/**
		 * Retorna um elemento da posição index do vetor
		 */
		T elementAt(int index) {
			return this->elementData[index];
		}
		
		/**
		 * Retorna um elemento da posição index do vetor. Igual ao método elementAt(int)
		 */
		T get(int index) {
			return this->elementData[index];
		}		
		
		/**
		 * Confere se um index é válido
		 */
		bool validIndex(int index) {
			return index >= 0 && index < this->elementCount ? true : false;
		}
		
		/**
		 * Confere se o vetor está vazio
		 */
		bool isEmpty() {
			return this->elementCount > 0 ? false : true;
		}

		/**
		 * Remove e retorna o elemento da posição index do vetor
		 */
		T remove(int index) {
			if (!this->isEmpty() && this->validIndex(index)) {
				T ret = this->elementData[index];
				
				for (int i = index; i < this->elementCount-1; i++)
					this->elementData[i] = this->elementData[i+1];

				this->elementData = (T *) realloc(this->elementData, sizeof(T) * --this->elementCount);
				return ret;
			} else
				return NULL;
		}

		/**
		 * Remove o elemento da posição index do vetor
		 */		
		void removeElementAt(int index) {
			if (!this->isEmpty() && this->validIndex(index)) {
				for (int i = index; i < this->elementCount-1; i++)
					this->elementData[i] = this->elementData[i+1];

				this->elementData = (T *) realloc(this->elementData, sizeof(T) * --this->elementCount);
			}
		}

		/**
		 * Retorna o tamanho atual do vetor
		 */
		int size() {
			return this->elementCount;
		}
		
		/**
		 * Aloca e garante um tamanho mínimo
		 */
		void ensureCapacity(int min) {
			if (min > 0) {
				this->elementCount = min;
				this->elementData = (T *) realloc(this->elementData, sizeof(T) * min);
			}
		}
		
		/**
		 * Define o tamanho do vetor
		 */
		void setSize(int size) {
			if (size >= 0) {
				this->ensureCapacity(size);
			}
		}
		
		/**
		 * Remove todos elementos do vetor
		 */
		void clear() {
			if (this->elementData != NULL) {
				free(this->elementData);
				this->elementData = NULL;
			}
			this->elementCount = 0;
	
		}
		
		/**
		 * Copia o conteúdo deste vetor para outro vetor
		 */
		void copyInto(Vector<T> *v, int start) {
			for (int i = start, thisSize = this->size(), j = 0, vSize = v->size(); i < thisSize && j < vSize; i++, j++) {
				v->add(j, this->get(i));
			}
		}
};

#endif

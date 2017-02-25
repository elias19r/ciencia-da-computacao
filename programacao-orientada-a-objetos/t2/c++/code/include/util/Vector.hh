#ifndef _VECTOR_HH_
#define _VECTOR_HH_

#include <stdlib.h>
#include <string.h>
#include "../util/Object.hh"

template <class T>
class Vector : public Object {
	private:
		int elementCount;
		T *elementData;
	public:
		Vector() {
			this->elementCount = 0;
			this->elementData  = NULL;
		}


		Vector(int initialCapacity) {
			this->elementCount = 0;
			this->elementData  = NULL;
			this->ensureCapacity(initialCapacity);
		}


		Vector(Vector<T> *vector) {
			this->elementCount = vector->elementCount;
			this->elementData = (T *) calloc(this->elementCount, sizeof(T));
			memcpy(this->elementData, vector->elementData, this->elementCount);
		}


		virtual ~Vector() {
			if (this->elementData != NULL)
				free(this->elementData);
		}


		void add(T element) {
			this->elementData = (T *) realloc(this->elementData, sizeof(T) * (this->elementCount + 1));
			this->elementData[this->elementCount++] = element;
		}


		bool add(int index, T element) {
			if (this->validIndex(index)) {
				this->elementData[index] = element;
				return true;
			}

			return false;
		}


		T elementAt(int index) {
			return this->elementData[index];
		}


		T get(int index) {
			return this->elementData[index];
		}		


		bool validIndex(int index) {
			return index >= 0 && index < this->elementCount ? true : false;
		}


		bool isEmpty() {
			return this->elementCount > 0 ? false : true;
		}


		T remove(int index) {
			if (!this->isEmpty() && this->validIndex(index)) {
				T ret = this->elementData[index];
		
				for (int i = index; i < this->elementCount-1; ++i)
					this->elementData[i] = this->elementData[i+1];

				this->elementData = (T *) realloc(this->elementData, sizeof(T) * --this->elementCount);
				return ret;
			}

			return NULL;
		}


		void removeElementAt(int index) {
			if (!this->isEmpty() && this->validIndex(index)) {
				for (int i = index; i < this->elementCount-1; ++i)
					this->elementData[i] = this->elementData[i+1];

				this->elementData = (T *) realloc(this->elementData, sizeof(T) * --this->elementCount);
			}

		}

		int size() {
			return this->elementCount;
		}


		void ensureCapacity(int min) {
			if (min > 0) {
				this->elementCount = min;
				this->elementData = (T *) realloc(this->elementData, sizeof(T) * min);
			}
		}


		void setSize(int size) {
			if (size >= 0) {
				this->ensureCapacity(size);
			}
		}


		void clear() {
			if (this->elementData != NULL) {
				free(this->elementData);
				this->elementData = NULL;
			}

			this->elementCount = 0;
		}


		void copyInto(Vector<T> *v, int start) {
			for (int i = start, thisSize = this->size(), j = 0, vSize = v->size(); i < thisSize && j < vSize; ++i, ++j)
				v->add(j, this->get(i));
		}


		Vector<T> *clone() {
			return new Vector<T>(this);
		}
};

#endif

#ifndef _RELATIONSHIP_H_
#define _RELATIONSHIP_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Classe Relationship
 * Armazena e manipula informações de um relacionamento entre usuários
 */
class Relationship {
	private:
		int fieldSize;
		
		char *userEmail;
		char *contactEmail;
		int contactOffset;

		int fileOffset;
		bool seek;
		
	public:
		/**
		 * Construtor
		 */
		Relationship(int size) {
			this->fieldSize = size;
			
			this->userEmail = (char *) calloc(1, size + sizeof(char));
			this->contactEmail = (char *) calloc(1, size + sizeof(char));
			this->contactOffset = -1;

			this->fileOffset = 0;
			this->seek = false;
		}
		
		/**
		 * Destrutor
		 */	
		~Relationship() {
			free(this->userEmail);
			free(this->contactEmail);
		}

		/**
		 * Lê um relacionamento de um arquivo de dados
		 */		
		void read(FILE *fpDAT) {
			if (this->seek) {
				if (this->fileOffset < 0)
					fseek(fpDAT, this->fileOffset, SEEK_END);
				else
					fseek(fpDAT, this->fileOffset, SEEK_SET);
			}
				
			fread(this->userEmail, this->fieldSize, 1, fpDAT);
			fread(this->contactEmail, this->fieldSize, 1, fpDAT);
			fread(&(this->contactOffset), sizeof(int), 1, fpDAT);
			
			this->userEmail[this->fieldSize] = '\0';
			this->contactEmail[this->fieldSize] = '\0';	
		}
		
		/**
		 * Escreve um relacionamento no arquivo de dados
		 */
		void write(FILE *fpDAT) {
			if (this->seek) {
				if (this->fileOffset < 0)
					fseek(fpDAT, this->fileOffset, SEEK_END);
				else
					fseek(fpDAT, this->fileOffset, SEEK_SET);
			}

			fwrite(this->userEmail, this->fieldSize, 1, fpDAT);
			fwrite(this->contactEmail, this->fieldSize, 1, fpDAT);
			fwrite(&(this->contactOffset), sizeof(int), 1, fpDAT);
		}
		
		/**
		 * GETTERS
		 */	
		
		char *user() { return this->userEmail; }
		char *contact() { return this->contactEmail; }
		int offset() { return this->contactOffset; }
		
		/**
		 * SETTERS
		 */		
		
		void user(char *email) {
			strncpy(this->userEmail, email, this->fieldSize);
		}

		void contact(char *email) {
			strncpy(this->contactEmail, email, this->fieldSize);
		}

		void offset(int value) {
			if (value >=0) this->contactOffset = value;
		}				
		
		void at(int value) {
			this->fileOffset = value;
			this->seek = true;
		}
		
		void dontSeek() {
			this->seek = false;
		}
};

#endif

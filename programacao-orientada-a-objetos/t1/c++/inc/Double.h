#ifndef _DOUBLE_H_
#define _DOUBLE_H_

/**
 * Classe Double
 * Armazena e manipula um número real
 */
class Double {
	private:
		double d;

	public:
		/**
		 * Construtor
		 */
		Double(double value) {
			this->set(value);
		}
		
		/**
		 * Destrutor
		 */
		~Double() {}
		
		/**
		 * SETTER
		 */
		void set(double value) {
			this->d = value;
		}
};

#endif

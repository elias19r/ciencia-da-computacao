/**
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 *
 * SCC0241 Laboratório de Bases de Dados
 * Turma 3
 *
 * Trabalho Prático 3 - "PL-SQL"
 * Script PL-SQL
 */

--=============================================================================

-- Comando que indica a língua na qual será executado o script e configura o uso de caracteres especiais
ALTER SESSION SET NLS_LANGUAGE= 'PORTUGUESE' NLS_TERRITORY= 'BRAZIL';

-- Comando para evitar problemas com a criação incorreta de sequências
ALTER SESSION SET deferred_segment_creation = FALSE;

--=============================================================================

/**
 * Problema 1:
 *
 * Precisa-se criar um relatório para cada edição dos eventos.
 * Cada relatório deve conter o código, nome e descrição do evento a qual a edição
 * pertence; da edição deve-se guardar o código, a descrição, os nomes dos
 * patrocinadores e os totais de: artigos apresentados, participantes, verba e
 * gastos com despesas. Esses relatórios devem ser armazenados no banco de dados.
 */

/**
 * Solução para o Problema 1:
 * 
 * Criar uma tabela que armazene os relatórios.
 * Depois, com um programa PL/SQL, consultar os dados requeridos para
 * o relatório, organizá-los e inseri-los na tabela de relatórios.
 */

/* Criação da tabela de relatórios */

-- Drop de tabela
DROP TABLE relatorio;

/**
 * Tabela relatorio
 * @codEv   chave primária
 * @numEv   chave primária
 *
 * codEv                      código do evento
 * nomeEv                     nome do evento
 * descricaoEv                descrição do evento
 * numEd                      número da edição da edição
 * descricaoEd                descrição da edição
 * totalArtigos               total de artigos apresentados
 * totalParticipantes         total de participantes
 * totalVerba                 total de verba arrecadada
 * totalAuxilio               total de gastos com auxilio
 * totalDespesa               total de gastos com despesas
 * totalGasto                 total gasto
 * nomePatrocinadores         nomes dos patrocinadores
 *
 * @PK_RELAT  restrição de chave primária
 */
CREATE TABLE relatorio(
	codEv               NUMBER(5)      NOT NULL,
	nomeEv              VARCHAR2(50)   NOT NULL,
	descricaoEv         VARCHAR2(300),
	numEd               NUMBER(5),
	descricaoEd         VARCHAR2(300),
	totalArtigos        NUMBER(5),
	totalParticipantes  NUMBER(5),
	totalVerba          NUMBER(9,2),
	totalAuxilio        NUMBER(9,2),
	totalDespesa        NUMBER(9,2),
	totalGasto          NUMBER(9,2),
	nomePatrocinadores  VARCHAR2(300),

	CONSTRAINT PK_RELAT PRIMARY KEY(codEv, numEd)
);

/* Programa PL/SQL para consultar e inserir dados dos relatórios */
SET SERVEROUTPUT ON;
DECLARE
	-- Variáveis para os valores de cada campo da tabela relatorio
	var_codEv               relatorio.codEv%TYPE;
	var_nomeEv              relatorio.nomeEv%TYPE;
	var_descricaoEv         relatorio.descricaoEv%TYPE;
	var_numEd               relatorio.numEd%TYPE;
	var_descricaoEd         relatorio.descricaoEd%TYPE;
	var_totalArtigos        relatorio.totalArtigos%TYPE;
	var_totalParticipantes  relatorio.totalParticipantes%TYPE;
	var_totalVerba          relatorio.totalVerba%TYPE;
	var_totalAuxilio        relatorio.totalAuxilio%TYPE;
	var_totalDespesa        relatorio.totalDespesa%TYPE;
	var_totalGasto          relatorio.totalGasto%TYPE;
	var_nomePatrocinadores  relatorio.nomePatrocinadores%TYPE;

	-- Cursor que busca eventos
	CURSOR cur_ev IS
	(
		SELECT codEv, nomeEv, descricaoEv
		FROM evento
	);
	
	-- Cursor que busca edições de um dado evento
	CURSOR cur_ed(in_codEv NUMBER) IS
	(
		SELECT codEv, numEd, descricaoEd, qtdArtigosApresentadosEd, taxaEd
		FROM edicao
		WHERE codEv = in_codEv
	);
	
	-- Cursor para consultar o total de participantes em uma dada edição de um evento
	CURSOR cur_totalParticipantes(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT COUNT(idPart) AS totalParticipantes
		FROM inscrito
		WHERE codEv = in_codEv AND numEd = in_numEd
	);
	
	-- Cursor para consultar o total de patrocínios de uma dada edição de um evento
	CURSOR cur_totalPatrocinios(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT SUM(valorPat) AS totalPatrocinios
		FROM patrocinio
		WHERE codEv = in_codEv AND numEd = in_numEd
	);

	-- Cursor para consultar o total gasto com auxilios em uma dada edição de um evento
	CURSOR cur_totalAuxilio(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT SUM(valorAux) AS totalAuxilio
		FROM auxilio
		WHERE codEvApr = in_codEv AND numEdApr = in_numEd
	);
	
	-- Cursor para consultar o total gasto em despesas em uma dada edição de um evento
	CURSOR cur_totalDespesa(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT SUM(valorDesp) AS totalDespesa
		FROM despesa
		WHERE codEv = in_codEv AND numEd = in_numEd
	);
	
	-- Cursor que retorna o nome de patrocinadores em uma dada edição de um evento
	CURSOR cur_nomePatrocinador(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT p.razaoSocialPat AS nomePatrocinador
		FROM patrocinio pa JOIN patrocinador p ON pa.cnpjPat = p.cnpjPat
		WHERE pa.codEv = in_codEv AND pa.numEd = in_numEd
	);

	-- Registros para os respectivos cursores
	reg_ev                 cur_ev%ROWTYPE;
	reg_ed                 cur_ed%ROWTYPE;
	reg_totalParticipantes cur_totalParticipantes%ROWTYPE;
	reg_totalPatrocinios   cur_totalPatrocinios%ROWTYPE;
	reg_totalAuxilio       cur_totalAuxilio%ROWTYPE;
	reg_totalDespesa       cur_totalDespesa%ROWTYPE;
	reg_nomePatrocinador   cur_nomePatrocinador%ROWTYPE;

BEGIN
	-- Itera para cada evento
	OPEN cur_ev;
	LOOP
		FETCH cur_ev INTO reg_ev;
		EXIT WHEN cur_ev%NOTFOUND;

		-- Itera para cada edição
		OPEN cur_ed(reg_ev.codEv);
		LOOP
			FETCH cur_ed INTO reg_ed;
			EXIT WHEN cur_ed%NOTFOUND;
			
			-- Recebe nas variáveis os dados de interesse sobre o evento e edição
			var_codEv := reg_ev.codEv;
			var_nomeEv := reg_ev.nomeEv;
			var_descricaoEv := reg_ev.descricaoEv;
			var_numEd := reg_ed.numEd;
			var_descricaoEd := reg_ed.descricaoEd;
			var_totalArtigos := reg_ed.qtdArtigosApresentadosEd;

			-- Abre cursores e computa os dados
			OPEN cur_totalParticipantes(reg_ed.codEv, reg_ed.numEd);
			OPEN cur_totalPatrocinios(reg_ed.codEv, reg_ed.numEd);
			OPEN cur_totalAuxilio(reg_ed.codEv, reg_ed.numEd);
			OPEN cur_totalDespesa(reg_ed.codEv, reg_ed.numEd);
			OPEN cur_nomePatrocinador(reg_ed.codEv, reg_ed.numEd);

			-- Recebe dados de interesse nas variáveis
			FETCH cur_totalParticipantes INTO reg_totalParticipantes;
			var_totalParticipantes := reg_totalParticipantes.totalParticipantes;

			FETCH cur_totalPatrocinios INTO reg_totalPatrocinios;
			var_totalVerba := reg_totalPatrocinios.totalPatrocinios + (var_totalParticipantes * reg_ed.taxaEd);

			FETCH cur_totalAuxilio INTO reg_totalAuxilio;
			var_totalAuxilio := reg_totalAuxilio.totalAuxilio;

			FETCH cur_totalDespesa INTO reg_totalDespesa;
			var_totalDespesa := reg_totalDespesa.totalDespesa;

			-- Computa o total gasto = gastos com auxilio + gastos com despesas
			var_totalGasto := var_totalAuxilio + var_totalDespesa;

			-- Cria string com os nomes dos patrocinadores desta edição deste evento
			-- O nome dos patrocinadores são separados por ponto-e-virgula
			var_nomePatrocinadores := '';
			LOOP
				FETCH	cur_nomePatrocinador INTO reg_nomePatrocinador;
				EXIT WHEN cur_nomePatrocinador%NOTFOUND;
				var_nomePatrocinadores := reg_nomePatrocinador.nomePatrocinador || ';' || var_nomePatrocinadores;
			END LOOP;

			-- Tendo todos os dados prontos, cadastra o relatorio na tabela
			INSERT INTO relatorio VALUES
			(
				var_codEv,
				var_nomeEv,
				var_descricaoEv,
				var_numEd,
				var_descricaoEd,
				var_totalArtigos,
				var_totalParticipantes,
				var_totalVerba,
				var_totalAuxilio,
				var_totalDespesa,
				var_totalGasto,
				var_nomePatrocinadores
			);

			-- Fecha os cursores
			CLOSE cur_totalParticipantes;
			CLOSE cur_totalPatrocinios;
			CLOSE cur_totalAuxilio;
			CLOSE cur_totalDespesa;
			CLOSE cur_nomePatrocinador;

		END LOOP;
		CLOSE cur_ed;

	END LOOP;

	CLOSE cur_ev;
	
	-- Exceções pré-definidas
	EXCEPTION
	WHEN STORAGE_ERROR
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Memória insuficiente para operação.');
	WHEN PROGRAM_ERROR
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Erro interno no PL/SQL.');	
	WHEN TOO_MANY_ROWS
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: A consulta resultou em mais de uma linha.');
	WHEN DUP_VAL_ON_INDEX
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Relatório já existente.');
	WHEN OTHERS
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Houve um erro durante a excecução do programa.');
END;
/
/* 
-----------------
SAIDA DO PROGRAMA
-----------------

table RELATORIO created.
anonymous block completed

*/
/
--=============================================================================
/
/**
 * Problema 2:
 *
 * Devido a problemas com spam, pede-se que todos os e-mails das pessoas
 * cadastradas na base de dados tenha o símbolo @ (arroba) substituído
 * por # (cerquilha) para que possa ser impresso nas páginas web do sistema
 * de maneira a pelo menos atrapalhar bots de internet que fazem
 * scanning por endereços de e-mail.
 */
/
/**
 * Solução para o Problema 2:
 * 
 * Criar um programa Pl/SQL que, para cada pessoa cadastrada no banco de dados,
 * selecione o campo de e-mail, aplique uma operação de "replace" e atualize o
 * registro com o e-mail alterado (símbolo @ substituído por #)
 */
/
/* Programa PL/SQL */
SET SERVEROUTPUT ON;
DECLARE
	-- Exceções para este programa
	ex_sem_email  EXCEPTION;
	ex_sem_arroba EXCEPTION;

	-- Cursor para iterar pelas pessoas cadastradas no banco de dados
	CURSOR cur_pe IS
	(
		SELECT idPe, emailPe
		FROM pessoa
	);
	
	-- Registros para os respectivos cursores
	reg_pe         cur_pe%ROWTYPE;
	
	-- Variáveis auxiliares
	var_arroba_pos     NUMBER;
	var_new_emailPe    pessoa.emailPe%TYPE;
BEGIN
	-- Itera para todas as pessoas
	OPEN cur_pe;
	LOOP
		FETCH cur_pe INTO reg_pe;
		EXIT WHEN cur_pe%NOTFOUND;

		-- Confere por problemas (exceções)
		IF (reg_pe.emailPe IS NULL) THEN
			DBMS_OUTPUT.PUT_LINE('idPe = ' || reg_pe.idPe);
			RAISE ex_sem_email;
		END IF;

		var_arroba_pos := INSTR(reg_pe.emailPe, '@');
		IF (var_arroba_pos = 0) THEN
			DBMS_OUTPUT.PUT_LINE('idPe = ' || reg_pe.idPe);
			RAISE ex_sem_arroba;
		END IF;

		-- Aplica substituição no email da pessoa selecionada
		var_new_emailPe := REPLACE(reg_pe.emailPe, '@', '#');

		-- Atualiza o registro dessa pessoa com o novo email
		UPDATE pessoa SET
			emailPe = var_new_emailPe
		WHERE
			idPe = reg_pe.idPe;

	END LOOP;

	CLOSE cur_pe;

	-- Trata exceções deste programa
	EXCEPTION
	WHEN ex_sem_email
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Encontrada pessoa sem e-mail registrado.');
	WHEN ex_sem_arroba
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Encontrada pessoa com e-mail sem símbolo arroba.');

	-- Trata exceções pré-definidas
	WHEN STORAGE_ERROR
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Memória insuficiente para operação.');
	WHEN PROGRAM_ERROR
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Erro interno no PL/SQL.');	
	WHEN TOO_MANY_ROWS
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: A consulta resultou em mais de uma linha.');
	WHEN NO_DATA_FOUND
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: E-mail invalido ou pessoa não encontrada.');
	WHEN OTHERS
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Houve um erro durante a excecução do programa.');
END;
/
/* 
-----------------
SAIDA DO PROGRAMA
-----------------

anonymous block completed

*/
/
--=============================================================================
/
/**
 * Problema 3:
 * 
 * Quando há a inserção de novos dados, os atributos derivados presentes no banco devem ser atualizados.
 * Como ainda não existe a utilização de gatilhos (triggers) nesse BD, a verificação da corretude desses
 * atributos precisa ser realizada. Esse programa em PL/SQL realiza todas as verificações
 * e gera uma exceção se não há conformidade de valores.
 * 
 * Os atributos derivados nesse caso são:
 * 	Na tabela Patrocinio: saldoPat;
 * 	Evento: totalArtigosApresentadosEv;
 * 	Edição: saldoFinanceiroEd e qtdArtigosApresentadosEd
 */
/
/* Programa PL/SQL */
SET SERVEROUTPUT ON;
DECLARE
	-- Exceções para este programa
	ex_saldo_ed_errado         EXCEPTION;
	ex_total_arts_ed_errado    EXCEPTION;
	ex_saldo_pat_errado        EXCEPTION;
	ex_total_arts_ev_errado    EXCEPTION;

	-- Variáveis para guardar os valores de interesse

	-- Variáveis necessárias para o cálculo do valor do atributo derivado saldoFinanceiroEd
	var_codEv               evento.codEv%TYPE;
	var_numEd               edicao.numEd%TYPE;
	var_totalPatrocinios    patrocinio.valorPat%TYPE;
	var_taxaInscricao       edicao.taxaEd%TYPE;
	var_numInscritos        NUMBER(8);
	var_totalDespesas       despesa.valorDesp%TYPE;
	var_totalAuxilios       auxilio.valorAux%TYPE;
	var_saldo               edicao.saldoFinanceiroEd%TYPE;
	var_total               edicao.saldoFinanceiroEd%TYPE;

--=============================================================================

	-- Variáveis necessárias para o cálculo do valor do atributo derivado qtdArtigosApresentadosEd 
	var_numArtsEd          edicao.qtdArtigosApresentadosEd%TYPE;
	var_qtdArtsEd          edicao.qtdArtigosApresentadosEd%TYPE;

--=============================================================================

	-- Variáveis necessárias para o cálculo do valor do atributo derivado totalArtigosApresentadosEv 
	var_numArtsEv          evento.totalArtigosApresentadosEv%TYPE;
	var_totalArtsEv        evento.totalArtigosApresentadosEv%TYPE;

--=============================================================================

	-- Variáveis necessárias para o cálculo do valor do atributo derivado saldoPat 
	var_cnpjPat             patrocinio.cnpjPat%TYPE;
	var_valorPat            patrocinio.valorPat%TYPE;
	var_despesasPat         despesa.valorDesp%TYPE;
	var_auxiliosPat         auxilio.valorAux%TYPE;
	var_totalPat            patrocinio.saldoPat%TYPE;
	var_saldoPat            patrocinio.saldoPat%TYPE;

--=============================================================================
	-- Cursores necessários para o cálculo do valor do atributo derivado saldoFinanceiroEd

	-- Cursor para iterar os eventos
	CURSOR cur_eventos IS
	(
		SELECT codEv
		FROM evento
	);

	-- Cursor para iterar as edições de um dado evento
	CURSOR cur_edicoes(in_codEv NUMBER) IS
	(
		SELECT codEv, numEd
		FROM edicao
		WHERE codEv = in_codEv
	);

	-- Cursor para computar o total de patrocínios de uma edição de um evento
	CURSOR cur_patrocinios(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT  SUM(valorPat) AS totalPatrocinios
			FROM patrocinio
			WHERE codEv = in_codEv AND numEd = in_numEd
	);

	-- Cursor para computar as taxas de inscrições nas edições dos eventos 
	CURSOR cur_inscricoes(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT taxaEd
			FROM edicao
			WHERE codEv = in_codEv AND numEd = in_numEd
	);

	-- Cursor para computar a quantidade de inscritos em uma edição de um evento
	CURSOR cur_inscritos(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT COUNT(idPart) AS numIns
			FROM inscrito
			WHERE codEv = in_codEv AND numEd = in_numEd
	);

	-- Cursor para computar o total de despesas de uma edição de um evento
	CURSOR cur_despesas(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT SUM(valorDesp) AS totalDespesas
			FROM despesa 
			WHERE codEv = in_codEv AND numEd = in_numEd
	);
	
	-- Cursor para computar o total de auxílios de uma edição de um evento
	CURSOR cur_auxilios(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT  SUM(valorAux) AS totalAuxilios
			FROM auxilio
			WHERE codEvApr = in_codEv AND numEdApr = in_codEV
	);

	-- Cursor para computar o saldo financeiro de uma edição de um evento
	CURSOR cur_saldo(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT saldoFinanceiroEd AS saldo
			FROM edicao
			WHERE codEv = in_codEv AND numEd = in_numEd
	);
--=============================================================================

	-- Cursores necessários para o cálculo do valor do atributo derivado qtdArtigosApresentadosEd

	-- Cursor para a quantidade de artigos de uma edição de um evento
	CURSOR cur_numArtigosEd(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT COUNT(idArt) AS numArtsEd
			FROM artigo
			WHERE codEv = in_codEv AND numEd = in_numEd
	);

	-- Cursor para retorna a quantidade de artigos apresentados numa edição de um evento
	CURSOR cur_qtdArtigosApresentadosEd(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT qtdArtigosApresentadosEd
			FROM edicao
			WHERE codEv = in_codEv AND numEd = in_numEd
	);

--=============================================================================

	-- Cursor necessário para o cálculo do valor do atributo derivado totalArtigosApresentadosEv

	-- Cursor para o total de artigos apresentados em um evento
	CURSOR cur_totalArtigosApresentadosEv(in_codEv NUMBER) IS
	(
		SELECT totalArtigosApresentadosEv
			FROM evento
			WHERE codEv = in_codEv
	);

--=============================================================================

	-- Cursores necessários para o cálculo do valor do atributo derivado saldoPat
	
	-- Cursor para iterar todos os patrocinadores da edição do evento em questão
	CURSOR cur_cnpjPat(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT cnpjPat
			FROM patrocinio
			WHERE codEv = in_codEv AND numEd = in_numEd
	);
	
	-- Cursor para o valor do patrocínio de um patrocinador em uma dada edição de um evento
	CURSOR cur_valorPat(in_codEv NUMBER, in_numEd NUMBER, in_cnpjPat patrocinio.cnpjPat%TYPE) IS
	(
		SELECT valorPat
			FROM patrocinio
			WHERE codEv = in_codEv AND numEd = in_numEd AND cnpjPat = in_cnpjPat
	);

	-- Cursor para as despesas associadas a um patrocínio em uma dada edição de um evento
	CURSOR cur_despesasPat(in_codEv NUMBER, in_numEd NUMBER, in_cnpjPat patrocinio.cnpjPat%TYPE) IS
	(
		SELECT SUM(valorDesp) AS totalDespesas
			FROM despesa 
			WHERE codEv = in_codEv AND numEd = in_numEd AND cnpjPat = in_cnpjPat
	);
	
	-- Cursor para os auxílios associados a um patrocínio em uma dada edição de um evento
	CURSOR cur_auxiliosPat(in_codEv NUMBER, in_numEd NUMBER, in_cnpjPat patrocinio.cnpjPat%TYPE) IS
	(
		SELECT SUM(valorAux) AS totalAuxilios
			FROM auxilio 
			WHERE codEvPat = in_codEv AND numEdPat = in_numEd AND cnpjPat = in_cnpjPat
	);
	
	-- Cursor para o saldo de um patrocínio em uma dada edição de um evento que já está calculado no banco. Será utilizado para a comparação
	CURSOR cur_saldoPat(in_codEv NUMBER, in_numEd NUMBER, in_cnpjPat patrocinio.cnpjPat%TYPE) IS
	(
		SELECT saldoPat
			FROM patrocinio
			WHERE codEv = in_codEv AND numEd = in_numEd AND cnpjPat = in_cnpjPat
	);


--=============================================================================

	-- Registros necessários para o cálculo do valor do atributo derivado saldoFinanceiroEd
	reg_eventos         cur_eventos%ROWTYPE;
	reg_edicoes         cur_edicoes%ROWTYPE;
	reg_patrocinios     cur_patrocinios%ROWTYPE;
	reg_inscricoes      cur_inscricoes%ROWTYPE;
	reg_inscritos       cur_inscritos%ROWTYPE;
	reg_despesas        cur_despesas%ROWTYPE;
	reg_auxilios        cur_auxilios%ROWTYPE;
	reg_saldo           cur_saldo%ROWTYPE;

--=============================================================================

	-- Registros necessários para o cálculo do valor do atributo derivado qtdArtigosApresentadosEd
	reg_numArtigosEd              cur_numArtigosEd%ROWTYPE;
	reg_qtdArtigosApresentadosEd  cur_qtdArtigosApresentadosEd%ROWTYPE;

--=============================================================================

	-- Registro necessário para o cálculo do valor do atributo derivado totalArtigosApresentadosEv
	reg_totalArtigosApresentadosEv   cur_totalArtigosApresentadosEv%ROWTYPE;
	
--=============================================================================

	-- Registros necessários para o cálculo do valor do atributo derivado saldoPat 
	reg_cnpjPat            cur_cnpjPat%ROWTYPE;
	reg_valorPat           cur_valorPat%ROWTYPE;
	reg_despesasPat        cur_despesasPat%ROWTYPE;
	reg_auxiliosPat        cur_auxiliosPat%ROWTYPE;
	reg_saldoPat           cur_saldoPat%ROWTYPE;

--=============================================================================

BEGIN
	-- Itera para cada evento
	OPEN cur_eventos;
	LOOP
		-- Inicializa a variavel para ser utilizada no cálculo do número de artigos apresentação no evento
		var_numArtsEv := 0;
		
		FETCH cur_eventos INTO reg_eventos;
		EXIT WHEN cur_eventos%NOTFOUND;

		var_codEv := reg_eventos.codEv;

		-- Itera para cada edição do evento em questão
		OPEN cur_edicoes(reg_eventos.codEv);
		LOOP
			FETCH cur_edicoes INTO reg_edicoes;
			EXIT WHEN cur_edicoes%NOTFOUND;

			var_numEd := reg_edicoes.numEd;
			
			-- Abre todos os cursores necessários para o cálculo do saldo financeiro da edição
			OPEN cur_patrocinios(reg_eventos.codEv, reg_edicoes.numEd);
			OPEN cur_inscricoes(reg_eventos.codEv, reg_edicoes.numEd);
			OPEN cur_inscritos(reg_eventos.codEv, reg_edicoes.numEd);
			OPEN cur_despesas(reg_eventos.codEv, reg_edicoes.numEd);
			OPEN cur_auxilios(reg_eventos.codEv, reg_edicoes.numEd);
			OPEN cur_saldo(reg_eventos.codEv, reg_edicoes.numEd);
		
			-- Atribui às variáveis os dados retornados
			FETCH cur_patrocinios INTO reg_patrocinios;
			var_totalPatrocinios := reg_patrocinios.totalPatrocinios;

			FETCH cur_inscricoes INTO reg_inscricoes;
			var_taxaInscricao := reg_inscricoes.taxaEd;
			 
			FETCH cur_inscritos INTO reg_inscritos;
			var_numInscritos := reg_inscritos.numIns;
			
			FETCH cur_despesas INTO reg_despesas;
			var_totalDespesas := reg_despesas.totalDespesas;
			
			FETCH cur_auxilios INTO reg_auxilios;
			var_totalAuxilios := reg_auxilios.totalAuxilios;

			FETCH cur_saldo INTO reg_saldo;
			var_saldo := reg_saldo.saldo;

			CLOSE cur_saldo;
			CLOSE cur_auxilios;
			CLOSE cur_despesas;
			CLOSE cur_inscritos;
			CLOSE cur_inscricoes;
			CLOSE cur_patrocinios;
			
			-- Calcula o valor do atributo derivado saldoFinanceiroEd segundo os dados cadastrados no banco
			var_total := var_totalPatrocinios + (var_taxaInscricao * var_numInscritos) - var_totalDespesas - var_totalAuxilios;
			
			-- Verifica se o valor cadastrado condiz com os dados
			IF (var_saldo = var_total)
				THEN
					DBMS_OUTPUT.PUT_LINE('O saldo financeiro da edição ' || var_numEd || ' do evento ' || var_codEv || ' está correto : R$ ' || to_char(var_total, 'FM999G999G999D90'));
				-- Caso os dados não estiverem corretos, gera uma exceção
				ELSE
					DBMS_OUTPUT.PUT_LINE('O saldo financeiro da edição ' || var_numEd || ' do evento ' || var_codEv || ' está errado. O valor correto : R$ ' || to_char(var_total, 'FM999G999G999D90'));
					RAISE ex_saldo_ed_errado;
			END IF;

--=============================================================================

			-- Abre todos os cursores necessários para o cálculo da quantidade de artigos apresentados na edição do evento
			OPEN cur_numArtigosEd(reg_eventos.codEv, reg_edicoes.numEd);
			OPEN cur_qtdArtigosApresentadosEd(reg_eventos.codEv, reg_edicoes.numEd);
			
			-- Atribui às variáveis os dados retornados
			FETCH cur_numArtigosEd INTO reg_numArtigosEd;
			var_numArtsEd := reg_numArtigosEd.numArtsEd;

			FETCH cur_qtdArtigosApresentadosEd INTO reg_qtdArtigosApresentadosEd;
			var_qtdArtsEd := reg_qtdArtigosApresentadosEd.qtdArtigosApresentadosEd;

			-- Verifica se o valor cadastrado condiz com os dados
			IF (var_numArtsEd = var_qtdArtsEd)
				THEN
					DBMS_OUTPUT.PUT_LINE('A quantidade de artigos apresentados na edição ' || var_numEd || ' do evento ' || var_codEv || ' está correta : ' || var_numArtsEd);
				-- Caso os dados não estiverem corretos, gera uma exceção
				ELSE
					DBMS_OUTPUT.PUT_LINE('A quantidade de artigos apresentados na edição ' || var_numEd || ' do evento ' || var_codEv || ' está errada. O valor correto : ' || var_numArtsEd);
					RAISE ex_total_arts_ed_errado;
			END IF;

			-- Acumula a quantidade de artigos apresentados em cada edição do evento para o cálculo do total de artigos apresentados no evento
			var_numArtsEv := var_numArtsEv + var_numArtsEd;
			
			CLOSE cur_qtdArtigosApresentadosEd;
			CLOSE cur_numArtigosEd;
--=============================================================================
			
			-- Itera para todos os patrocinadores da edição do evento
			OPEN cur_cnpjPat(reg_eventos.codEv, reg_edicoes.numEd);
			LOOP
				FETCH cur_cnpjPat INTO reg_cnpjPat;
				EXIT WHEN cur_cnpjPat%NOTFOUND;

				var_cnpjPat := reg_cnpjPat.cnpjPat;
				
				-- Abre todos os cursores necessários para o cálculo do saldo do patrocínio
				OPEN cur_valorPat(reg_eventos.codEv, reg_edicoes.numEd, var_cnpjPat);
				OPEN cur_despesasPat(reg_eventos.codEv, reg_edicoes.numEd, var_cnpjPat);
				OPEN cur_auxiliosPat(reg_eventos.codEv, reg_edicoes.numEd, var_cnpjPat);
				OPEN cur_saldoPat(reg_eventos.codEv, reg_edicoes.numEd, var_cnpjPat);

				-- Atribui às variáveis os dados retornados
				FETCH cur_valorPat INTO reg_valorPat;
				var_valorPat := reg_valorPat.valorPat;

				FETCH cur_despesasPat INTO reg_despesasPat;
				var_despesasPat := reg_despesasPat.totalDespesas;
				
				FETCH cur_auxiliosPat INTO reg_auxiliosPat;
				var_auxiliosPat := reg_auxiliosPat.totalAuxilios;

				FETCH cur_saldoPat INTO reg_saldoPat;
				var_saldoPat := reg_saldoPat.saldoPat;

				CLOSE cur_saldoPat;
				CLOSE cur_auxiliosPat;
				CLOSE cur_despesasPat;
				CLOSE cur_valorPat;

				-- Calcula o valor do atributo derivado saldoPat segundo os dados cadastrados no banco
				var_totalPat := var_valorPat - var_despesasPat - var_auxiliosPat;	

				-- Verifica se o valor cadastrado condiz com os dados
				IF (var_totalPat = var_saldoPat)
					THEN
						DBMS_OUTPUT.PUT_LINE('O saldo financeiro do patrocinio de CNPJ ' || var_cnpjPat|| ' está correto : R$ ' || to_char(var_totalPat, 'FM999G999G999D90'));
					-- Caso os dados não estiverem corretos, gera uma exceção
					ELSE
						DBMS_OUTPUT.PUT_LINE('O saldo financeiro do patrocinio de CNPJ ' || var_cnpjPat|| ' está errado. O valor correto : R$ ' || to_char(var_totalPat, 'FM999G999G999D90'));
						RAISE ex_saldo_pat_errado;
				END IF;

			END LOOP;
			CLOSE cur_cnpjPat;

		END LOOP;
		CLOSE cur_edicoes;

--=============================================================================
		-- Abre o cursor necessário para o cálculo do total de artigos apresentados no evento
		OPEN cur_totalArtigosApresentadosEv(reg_eventos.codEv);
		
		-- Atribui à variável o dado retornado
		FETCH cur_totalArtigosApresentadosEv INTO reg_totalArtigosApresentadosEv;
		var_totalArtsEv := reg_totalArtigosApresentadosEv.totalArtigosApresentadosEv;

		-- Verifica se o valor cadastrado condiz com os dados, que foi sendo calculado de acordo com o total de cada edição, que foi sendo acumulado no loop anterior
		IF (var_numArtsEv = var_totalArtsEv)
			THEN
				DBMS_OUTPUT.PUT_LINE('A quantidade de artigos apresentados no evento ' || var_codEv || ' está correta : ' || var_numArtsEv);
			-- Caso os dados não estiverem corretos, gera uma exceção
			ELSE
				DBMS_OUTPUT.PUT_LINE('A quantidade de artigos apresentados no evento ' || var_codEv || ' está errada. O valor correto : ' || var_numArtsEv);
				RAISE ex_total_arts_ev_errado;
		END IF;

		CLOSE cur_totalArtigosApresentadosEv;
--=============================================================================

	END LOOP;
	CLOSE cur_eventos;

	EXCEPTION
	
	-- Trata exceções para este programa
	WHEN ex_saldo_ed_errado
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Saldo financeiro da edição calculado errado.');
	WHEN ex_total_arts_ed_errado
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Total de artigos da edição calculado errado.');
	WHEN ex_saldo_pat_errado
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Saldo financeiro do patrocínio calculado errado.');
	WHEN ex_total_arts_ev_errado
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Total de artigos do evento calculado errado.');

	-- Exceções pré-definidas
	WHEN STORAGE_ERROR
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Memória insuficiente para operação.');
	WHEN PROGRAM_ERROR
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Erro interno no PL/SQL.');
	WHEN TOO_MANY_ROWS
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: A consulta resultou em mais de uma linha.');
	WHEN NO_DATA_FOUND
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: E-mail invalido ou pessoa não encontrada.');
	WHEN OTHERS
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Houve um erro durante a excecução do programa.');
END;

/* 
-----------------
SAIDA DO PROGRAMA
-----------------

anonymous block completed
O saldo financeiro da edição 1 do evento 1 está correto : R$ 11.300,00
A quantidade de artigos apresentados na edição 1 do evento 1 está correta : 15
O saldo financeiro do patrocinio de CNPJ 12.435.213/0221-10 está correto : R$ 1.700,00
O saldo financeiro do patrocinio de CNPJ 12.878.232/0131-28 está correto : R$ 1.700,00
O saldo financeiro do patrocinio de CNPJ 23.543.231/0211-02 está correto : R$ 2.600,00
O saldo financeiro do patrocinio de CNPJ 66.232.656/0111-03 está correto : R$ 2.600,00
O saldo financeiro da edição 2 do evento 1 está correto : R$ 31.900,00
A quantidade de artigos apresentados na edição 2 do evento 1 está correta : 15
O saldo financeiro do patrocinio de CNPJ 12.435.213/0221-10 está correto : R$ 6.700,00
O saldo financeiro do patrocinio de CNPJ 12.878.232/0131-28 está correto : R$ 6.700,00
O saldo financeiro do patrocinio de CNPJ 23.543.231/0211-02 está correto : R$ 7.600,00
O saldo financeiro do patrocinio de CNPJ 66.232.656/0111-03 está correto : R$ 7.600,00
O saldo financeiro da edição 3 do evento 1 está correto : R$ 7.900,00
A quantidade de artigos apresentados na edição 3 do evento 1 está correta : 15
O saldo financeiro do patrocinio de CNPJ 12.435.213/0221-10 está correto : R$ 700,00
O saldo financeiro do patrocinio de CNPJ 12.878.232/0131-28 está correto : R$ 700,00
O saldo financeiro do patrocinio de CNPJ 23.543.231/0211-02 está correto : R$ 1.600,00
O saldo financeiro do patrocinio de CNPJ 66.232.656/0111-03 está correto : R$ 1.600,00
A quantidade de artigos apresentados no evento 1 está correta : 45

*/


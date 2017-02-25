/**
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 * 
 * SCC0241 Laboratório de Bases de Dados
 * Turma 3
 * 
 * Trabalho Prático 4 - "scriptGatilho"
 */
/
/
-- Comando que indica a língua na qual será executado o script e configura o uso de caracteres especiais
ALTER SESSION SET NLS_LANGUAGE= 'PORTUGUESE' NLS_TERRITORY= 'BRAZIL';
/
-- Comando para evitar problemas com a criação incorreta de sequências
ALTER SESSION SET deferred_segment_creation = FALSE;
/
-- Direciona mensagens para a tela
SET SERVEROUTPUT ON;
/
-- =================== ESPECIFICAÇÃO DO PROBLEMA ==============================
/*
	Quando uma pessoa for inscrita/atualizada/desinscrita numa edição de um evento,
pede-se para ser emitido um relatório da quantidade de pessoas inscritas que são organizadores
naquela edição do evento. Caso a quantidade de organizadores não represente 5% da
quantidade de inscritos total, uma mensagem "PROBLEMA, quantidade de organizadores
insuficiente (abaixo de 5%)" deve ser impressa, caso contrário uma mensagem positiva.
*/
/
/
-- =================== SOLUÇÃO E EXPLICAÇÃO ===================================
/*
	O problema descrito acima pode ser resolvido com um TRIGGER que atua sobre as
operações de INSERT, DELETE ou UPDATE na tabela "inscrito". O problema da tabela mutante ocorre,
pois para ser emitido o relatório requerido, será necessário uma consulta SELECT
sobre as tabelas "inscrito" e "pessoa", ou seja, uma das relações base ("inscrito")
está envolvida na ativação TRIGGER.

	A utilização de COMPOUND TRIGGER resolve o problema, pois com esse tipo de TRIGGER
pode-se garantir em qual momento a ação desejada será executada. Ou seja, no caso do
problema proposto, deve-se executar a consulta SELECT somente depois que ocorreram todos
os comandos DML.
*/
/
CREATE OR REPLACE TRIGGER ctrig_verificaOrganizadores
FOR INSERT OR DELETE OR UPDATE
ON inscrito
COMPOUND TRIGGER

	-- Variáveis para o relatório
	var_totalInscritos        NUMBER;
	var_totalOrganizadores    NUMBER;
	in_codEv                  NUMBER;
	in_numEd                  NUMBER;

     -- Para ser executado antes dos comandos DML: Nada
	BEFORE STATEMENT IS
	BEGIN
		NULL;
	END BEFORE STATEMENT;

     -- Para ser executado antes de cada inserção de tupla: Nada
	BEFORE EACH ROW IS
	BEGIN
		NULL;
	END BEFORE EACH ROW;

     -- Para ser executado depois de cada inserção de tupla: Guardar o codigo do evento e numero da edição
	AFTER EACH ROW IS
	BEGIN
		in_codEv := :NEW.codEv;
		in_numEd := :NEW.numEd;
	END AFTER EACH ROW;

     -- Para ser executado depois dos comandos DML: Resolver o problema proposto
	AFTER STATEMENT IS
	BEGIN
		-- Seleciona total de inscritos
		SELECT COUNT(*) INTO var_totalInscritos
			FROM inscrito
				WHERE codEv = in_codEv AND numEd = in_numEd;

		-- Seleciona total de organizadores
		SELECT COUNT(*) INTO var_totalOrganizadores
			FROM inscrito ins JOIN pessoa pe ON ins.idPart = pe.idPe
				WHERE ins.codEv = in_codEv AND ins.numEd = in_numEd
					AND pe.tipoOrganizador = UPPER('O');

		-- Imprime informações do relatório de inscritos
		DBMS_OUTPUT.PUT_LINE('Relatório de Inscritos');
		DBMS_OUTPUT.PUT_LINE('----------------------');
		DBMS_OUTPUT.PUT_LINE('Código do Evento            : ' || in_codEv);
		DBMS_OUTPUT.PUT_LINE('Número da Edição            : ' || in_numEd);
		DBMS_OUTPUT.PUT_LINE('Total de Inscritos          : ' || var_totalInscritos);
		DBMS_OUTPUT.PUT_LINE('Quantidade de Organizadores : ' || var_totalOrganizadores);
		
		-- Confere se pelos menos 5% dos inscritos são organizadores
		-- 	e imprime mensagem da situação conforme proposto no problema
		IF (var_totalOrganizadores >= var_totalInscritos * 0.05) THEN
			DBMS_OUTPUT.PUT_LINE('Situação                    : OK, quantidade de organizadores suficiente (acima ou igual a 5%)');
		ELSE
			DBMS_OUTPUT.PUT_LINE('Situação                    : PROBLEMA, quantidade de organizadores insuficiente (abaixo de 5%)');
		END IF;
		
		EXCEPTION

		-- Exceções pré-definidas
		WHEN STORAGE_ERROR
			THEN DBMS_OUTPUT.PUT_LINE('ERRO: Memória insuficiente para operação.');
		WHEN PROGRAM_ERROR
			THEN DBMS_OUTPUT.PUT_LINE('ERRO: Erro interno no PL/SQL.');
		WHEN NO_DATA_FOUND
			THEN DBMS_OUTPUT.PUT_LINE('ERRO: Nenhum dado foi retornado nas consultas.');
		WHEN OTHERS
			THEN DBMS_OUTPUT.PUT_LINE('ERRO: Houve um erro durante a excecução do programa.');

	END AFTER STATEMENT;

END ctrig_verificaOrganizadores;
/
-- =================== DEFINIÇÃO DE TABELA MUTANTTE ===========================
/*
	Uma tabela A é considerada mutante se algum TRIGGER definido sobre a tabela A
executar comandos que leem ou modificam a própria tabela A.
*/
/

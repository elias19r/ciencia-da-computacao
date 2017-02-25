/**
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 * 
 * SCC0241 Laboratório de Bases de Dados
 * Turma 3
 * 
 * Trabalho Prático 4 - "scriptSubprograma"
 */
/
/
-- Comando que indica a língua na qual será executado o script e configura o uso de caracteres especiais
ALTER SESSION SET NLS_LANGUAGE= 'PORTUGUESE' NLS_TERRITORY= 'BRAZIL';
/
-- Comando para evitar problemas com a criação incorreta de sequências
ALTER SESSION SET deferred_segment_creation = FALSE;
/
/
-- ================== DECLARAÇÃO DAS FUNÇÕES =================================
/
/
/**
 * Pega os nomes de todos os patrocionadores de uma edição de um evento
 * Parâmetros:
 * 	@in_codEv    NUMBER    Código do evento
 * 	@in_numEd    NUMBER    Número da edição
 * Retorno
 * 	@ret_patrocinadores    VARCHAR2    Concatenação dos nomes dos patrocionadores
 */
CREATE OR REPLACE FUNCTION func_patrocionadores(in_codEv IN NUMBER, in_numEd IN NUMBER)
RETURN VARCHAR2
IS
	-- String para concatenar os nomes
	ret_patrocinadores    VARCHAR2(300);

	-- Cursor que retorna um nome de patrocinador em uma dada edição de um evento
	CURSOR cur_patrocinador(in_codEv NUMBER, in_numEd NUMBER) IS
	(
		SELECT p.razaoSocialPat AS patrocinador
			FROM patrocinio pa JOIN patrocinador p ON pa.cnpjPat = p.cnpjPat
				WHERE pa.codEv = in_codEv AND pa.numEd = in_numEd
	);

	-- Registro para o cursor do nome de patrocinador
	reg_patrocinador    cur_patrocinador%ROWTYPE;
BEGIN
	OPEN cur_patrocinador(in_codEv, in_numEd);
	ret_patrocinadores := '';
	LOOP
		FETCH	cur_patrocinador INTO reg_patrocinador;
		EXIT WHEN cur_patrocinador%NOTFOUND;
		ret_patrocinadores := reg_patrocinador.patrocinador || '; ' || ret_patrocinadores;
		-- O nome dos patrocinadores são separados por ponto-e-virgula
	END LOOP;
	
	CLOSE cur_patrocinador;
	RETURN ret_patrocinadores;
END func_patrocionadores;
/
/**
 * Calcula o total gasto com despesas em uma edição de um evento
 * Parâmetros:
 * 	@in_codEv    NUMBER    Código do evento
 * 	@in_numEd    NUMBER    Número da edição
 * Retorno
 * 	@ret_totalDespesas    NUMBER    Total gasto com despesas
 */
CREATE OR REPLACE FUNCTION func_totalDespesas(in_codEv IN NUMBER, in_numEd IN NUMBER)
RETURN NUMBER
IS
	var_totalAuxilio     NUMBER;
	var_totalDespesa     NUMBER;
	ret_totalDespesas    NUMBER;
BEGIN
	SELECT SUM(valorAux) INTO var_totalAuxilio
		FROM auxilio
			WHERE codEvApr = in_codEv AND numEdApr = in_numEd;

	SELECT SUM(valorDesp) INTO var_totalDespesa
		FROM despesa
			WHERE codEv = in_codEv AND numEd = in_numEd;
	
	ret_totalDespesas := var_totalAuxilio + var_totalDespesa;
	RETURN ret_totalDespesas;
END func_totalDespesas;
/
/**
 * Calcula a quantidade de participantes em uma edição de um evento
 * Parâmetros:
 * 	@in_codEv    NUMBER    Código do evento
 * 	@in_numEd    NUMBER    Número da edição
 * Retorno
 * 	@ret_qtdeParticipantes    NUMBER    Total de participantes
 */
CREATE OR REPLACE FUNCTION func_qtdeParticipantes(in_codEv IN NUMBER, in_numEd IN NUMBER)
RETURN NUMBER
IS
	ret_qtdeParticipantes    NUMBER;
BEGIN
	SELECT COUNT(idPart) INTO ret_qtdeParticipantes
		FROM inscrito
			WHERE codEv = in_codEv AND numEd = in_numEd;
	
	RETURN ret_qtdeParticipantes;
END func_qtdeParticipantes;
/
/
-- ================== DECLARAÇÃO DO PROCEDIMENTO ==============================
/
/
/**
 * Procedimento para a geração de relatórios de edições de um evento
 * Parâmetros:
 * 	@in_codEv    NUMBER    Código do evento
 * 	@in_numEd    NUMBER    Número da edição
 * Retorno
 * 	Imprime um relatório com nome e descrição do evento a qual a edição pertence,
 * 	descrição da edição, total de artigos, nomes dos patrocinadores, quantidade 
 * 	de participantes e total de gastos com despesas
 */
CREATE OR REPLACE PROCEDURE proc_relatorioEdicao(in_codEv IN NUMBER, in_numEd IN NUMBER)
IS
	-- Variavies para o relatório
	var_nomeEv               VARCHAR2(50);
	var_descricaoEv          VARCHAR2(300);
	var_descricaoEd          VARCHAR2(300);
	var_qtdeParticipantes    NUMBER;
	var_totalDespesas        NUMBER;
	var_totalArtigos         NUMBER;
	var_patrocionadores      VARCHAR2(300);
BEGIN
	-- Busca dados básicos sobre o evento e a edição
	SELECT ev.nomeEv, ev.descricaoEv, ed.descricaoEd, ed.qtdArtigosApresentadosEd
	INTO var_nomeEv, var_descricaoEv, var_descricaoEd, var_totalArtigos
		FROM evento ev JOIN edicao ed ON ev.codEv = ed.codEv
			AND ev.codEv = in_codEv AND ed.numEd = in_numEd;

	-- Faz chamadas às funções para receber os dados
	var_patrocionadores   := func_patrocionadores(in_codEv, in_numEd);
	var_qtdeParticipantes := func_qtdeParticipantes(in_codEv, in_numEd);
	var_totalDespesas     := func_totalDespesas(in_codEv, in_numEd);
	
	-- Imprime informações do relatório
	DBMS_OUTPUT.PUT_LINE('Evento');
	DBMS_OUTPUT.PUT_LINE('------');
	DBMS_OUTPUT.PUT_LINE('Código    : ' || in_codEv);
	DBMS_OUTPUT.PUT_LINE('Nome      : ' || var_nomeEv);
	DBMS_OUTPUT.PUT_LINE('Descrição : ' || var_descricaoEv);

	DBMS_OUTPUT.PUT_LINE('');
	DBMS_OUTPUT.PUT_LINE('Edição');
	DBMS_OUTPUT.PUT_LINE('------');
	DBMS_OUTPUT.PUT_LINE('Número    : ' || in_numEd);
	DBMS_OUTPUT.PUT_LINE('Descrição : ' || var_descricaoEd);

	DBMS_OUTPUT.PUT_LINE('');
	DBMS_OUTPUT.PUT_LINE('Relatório da Edição');
	DBMS_OUTPUT.PUT_LINE('-------------------');
	DBMS_OUTPUT.PUT_LINE('Total de Artigos            : ' || var_totalArtigos);
	DBMS_OUTPUT.PUT_LINE('Patrocionadores             : ' || var_patrocionadores);
	DBMS_OUTPUT.PUT_LINE('Quantidade de Participantes : ' || var_qtdeParticipantes);
	DBMS_OUTPUT.PUT_LINE('Total de Despesas           : ' || var_totalDespesas);

	EXCEPTION

	-- Exceções pré-definidas
	WHEN STORAGE_ERROR
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Memória insuficiente para operação.');
	WHEN PROGRAM_ERROR
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Erro interno no PL/SQL.');
	WHEN NO_DATA_FOUND
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Nenhum evento ou edição foi encontrada.');
	WHEN OTHERS
		THEN DBMS_OUTPUT.PUT_LINE('ERRO: Houve um erro durante a excecução do programa.');

END proc_relatorioEdicao;
/
/
-- ================== CHAMADAS AO PROCEDIMENTO ==============================
/
/
SET SERVEROUTPUT ON;
EXECUTE proc_relatorioEdicao(1, 1);
EXECUTE proc_relatorioEdicao(1, 2);
EXECUTE proc_relatorioEdicao(1, 3);
/
/
/
-- ================== RESULTADOS OBTIDOS ====================================
/*

anonymous block completed
Evento
------
Código    : 1
Nome      : Simpósio de Tecnologia da Informação (STI)
Descrição : O STI é um evento que ocorre anualmente no Brasil sobre tecnologia da informação reunindo acadêmicos e professionais técnicos da área.

Edição
------
Número    : 1
Descrição : Edição piloto do evento STI: WWW e Telecomunicações.

Relatório da Edição
-------------------
Total de Artigos            : 15
Patrocionadores             : Fundação de Amparo a Pesquisa; SysDesign; Itautec; IBM Brasil; 
Quantidade de Participantes : 30
Total de Despesas           : 65400

anonymous block completed
Evento
------
Código    : 1
Nome      : Simpósio de Tecnologia da Informação (STI)
Descrição : O STI é um evento que ocorre anualmente no Brasil sobre tecnologia da informação reunindo acadêmicos e professionais técnicos da área.

Edição
------
Número    : 2
Descrição : Segunda edição do evento STI: Tecnologias Assistivas.

Relatório da Edição
-------------------
Total de Artigos            : 15
Patrocionadores             : Fundação de Amparo a Pesquisa; SysDesign; Itautec; IBM Brasil; 
Quantidade de Participantes : 30
Total de Despesas           : 65400

anonymous block completed
Evento
------
Código    : 1
Nome      : Simpósio de Tecnologia da Informação (STI)
Descrição : O STI é um evento que ocorre anualmente no Brasil sobre tecnologia da informação reunindo acadêmicos e professionais técnicos da área.

Edição
------
Número    : 3
Descrição : Terceira edição do evento STI: Web móvel e Redes Descentralizadas

Relatório da Edição
-------------------
Total de Artigos            : 15
Patrocionadores             : Fundação de Amparo a Pesquisa; SysDesign; Itautec; IBM Brasil; 
Quantidade de Participantes : 30
Total de Despesas           : 65400

*/

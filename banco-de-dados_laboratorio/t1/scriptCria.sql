/**
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 * 
 * SCC0241 Laboratório de Bases de Dados
 * Turma 3
 * 
 * Trabalho Prático 1 - "Conferência Acadêmica"
 * Script de Criação das Tabelas
 */

-- Comando que indica a língua na qual será executado o script e configura o uso de caracteres especiais
ALTER SESSION SET NLS_LANGUAGE= 'PORTUGUESE' NLS_TERRITORY= 'BRAZIL';

-- Comando para evitar problemas com a criação incorreta de sequências
ALTER SESSION SET deferred_segment_creation = FALSE;

-- Drop em todas as tabelas, eliminando também suas restrições
DROP TABLE evento       CASCADE CONSTRAINTS;
DROP TABLE edicao       CASCADE CONSTRAINTS;
DROP TABLE pessoa       CASCADE CONSTRAINTS;
DROP TABLE inscrito     CASCADE CONSTRAINTS;
DROP TABLE artigo       CASCADE CONSTRAINTS;
DROP TABLE escreve      CASCADE CONSTRAINTS;
DROP TABLE organiza     CASCADE CONSTRAINTS;
DROP TABLE patrocinador CASCADE CONSTRAINTS;
DROP TABLE patrocinio   CASCADE CONSTRAINTS;
DROP TABLE despesa      CASCADE CONSTRAINTS;
DROP TABLE auxilio      CASCADE CONSTRAINTS;

-- Drop em todas as sequências
DROP SEQUENCE SEQ_CODEV_EVENTO;
DROP SEQUENCE SEQ_NUMED_EDICAO;
DROP SEQUENCE SEQ_IDPE_PESSOA;
DROP SEQUENCE SEQ_IDART_ARTIGO;
DROP SEQUENCE SEQ_CODDESP_DESPESA;

-- Criação das sequencias
CREATE SEQUENCE SEQ_CODEV_EVENTO    MINVALUE 0 START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE SEQ_NUMED_EDICAO    MINVALUE 0 START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE SEQ_IDPE_PESSOA     MINVALUE 0 START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE SEQ_IDART_ARTIGO    MINVALUE 0 START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE SEQ_CODDESP_DESPESA MINVALUE 0 START WITH 1 INCREMENT BY 1;

/**
 * Tabela evento
 * @codEv  chave primária
 * 
 * @codEv                       código do evento
 * @nomeEv                      nome do evento
 * @descricaoEv                 descrição do evento
 * @websiteEv                   website do evento
 * @totalArtigosApresentadosEv  número total de artigos apresentados no evento
 *
 * @PK_EVENTO  restrição de chave primária
 * @UN_EVENTO  restrição de chave secundária
 */
CREATE TABLE evento(
	codEv                      NUMBER(5)      NOT NULL,
	nomeEv                     VARCHAR2(50)   NOT NULL,
	descricaoEv                VARCHAR2(300),
	websiteEv                  VARCHAR2(50),
	totalArtigosApresentadosEv NUMBER(5),
	CONSTRAINT PK_EVENTO PRIMARY KEY(codEv),
	CONSTRAINT UN_EVENTO UNIQUE(nomeEv)
);

/**
 * Tabela edicao
 * @codEv, numEd  chave primária
 *
 * @codEv                     código do evento a qual pertence a edição
 * @numEd                     número da edição
 * @descricaoEd               descrição da edição
 * @dataInicioEd              data de início da edição
 * @dataFimEd                 data de término da edição
 * @localEd                   local da edição
 * @taxaEd                    taxa da edição
 * @saldoFinanceiroEd         saldo financeiro da edição
 * @qtdArtigosApresentadosEd  quantidade de artigos apresentados na edição
 *
 * @PK_EDICAO  restrição de chave primária
 * @FK_EDICAO  restrição de chave estrangeira com a tabela evento
 */
CREATE TABLE edicao(
	codEv                    NUMBER(5)      NOT NULL,
	numEd                    NUMBER(5)      NOT NULL,
	descricaoEd              VARCHAR2(300), 
	dataInicioEd             DATE,  --data inicio < data fim?
	dataFimEd                DATE,
	localEd                  VARCHAR2(50),
	taxaEd                   NUMBER(5,2),
	saldoFinanceiroEd        NUMBER(9,2),
	qtdArtigosApresentadosEd NUMBER(5),
	CONSTRAINT PK_EDICAO PRIMARY KEY(codEv, numEd),
	CONSTRAINT FK_EDICAO FOREIGN KEY(codEv) REFERENCES evento(codEv)
);

/**
 * Tabela pessoa
 * @idPe  chave primária
 *
 * @idPe              número identificador da pessoa
 * @nomePe            nome da pessoa
 * @emailPe           email da pessoa
 * @instituicaoPe     instituição da pessoa
 * @telefonePe        telefone da pessoa
 * @nacionalidadePe   nacionalidade da pessoa
 * @enderecoPe        endereço da pessoa
 * @tipoOrganizador   atributo 'flag' que caracteriza a pessoa como organizador
 * @tipoParticipante  atributo 'flag' que caracteriza a pessoa como participante
 * @tipoAutor         atributo 'flag' que caracteriza a pessoa como autor
 *
 * @PK_PESSOA  restrição de chave primária
 * @UN_PESSOA  restrição de chave secundária
 */
CREATE TABLE pessoa(
	idPe             NUMBER(5)     NOT NULL, 
	nomePe           VARCHAR2(50)  NOT NULL,
	emailPe          VARCHAR2(50)  NOT NULL,
	instituicaoPe    VARCHAR2(50),
	telefonePe       VARCHAR2(15),
	nacionalidadePe  VARCHAR2(20),
	enderecoPe       VARCHAR2(100),
	tipoOrganizador  CHAR(1),
	tipoParticipante CHAR(1),
	tipoAutor        CHAR(1),
	CONSTRAINT PK_PESSOA PRIMARY KEY(idPe),
	CONSTRAINT UN_PESSOA UNIQUE(emailPe)
);

/**
 * Tabela inscrito
 * @codEv, numEd, idPart  chave primária
 *
 * @codEv             código do evento a qual se está inscrito
 * @numEd             número da edição do evento a qual se está inscrito
 * @idPart            número de identificação do participante
 * @dataIns           data de inscrição no evento
 * @tipoApresentador  atributo 'flag' que caracteriza o inscrito como apresentador
 *
 * @PK_INSCRITO   restrição de chave primária
 * @FK_INSCRITO1  restrição de chave estrangeira com a tabela edicao
 * @FK_INSCRITO2  restrição de chave estrangeita com a tabela pessoa
 */
CREATE TABLE inscrito(
	codEv            NUMBER(5) NOT NULL, 
	numEd            NUMBER(5) NOT NULL, 
	idPart           NUMBER(5) NOT NULL, 
	dataInsc         DATE, 
	tipoApresentador CHAR(1),
	CONSTRAINT PK_INSCRITO  PRIMARY KEY(codEv, numEd, idPart), 
	CONSTRAINT FK_INSCRITO1 FOREIGN KEY(codEv, numEd) REFERENCES edicao(codEv, numEd),
	CONSTRAINT FK_INSCRITO2 FOREIGN KEY(idPart) REFERENCES pessoa(idPe)
);

/**
 * Tabela artigo
 * @idArt  chave primária
 *
 * @idArt         número identificador do artigo
 * @tituloArt     título do artigo
 * @dataApresArt  data de apresentação do artigo
 * @horaApresArt  hora de apresentação do artigo
 * @codEv         código do evento no qual o artigo será apresentado
 * @numEd         número da edição em que o artigo será apresentado
 * @idApr         número identificador do apresentador do artigo
 *
 * @PK_ARTIGO  restrição de chave primária
 * @FK_ARTIGO  restrição de chave estrangeira com a tabela inscrito
 */
CREATE TABLE artigo(
	idArt        NUMBER(5)      NOT NULL, 
	tituloArt    VARCHAR2(100), 
	dataApresArt DATE, 
	horaApresArt TIMESTAMP, 
	codEv        NUMBER(5), 
	numEd        NUMBER(5), 
	idApr        NUMBER(5),
	CONSTRAINT PK_ARTIGO PRIMARY KEY(idArt),
	CONSTRAINT FK_ARTIGO FOREIGN KEY(codEv, numEd, idApr) REFERENCES inscrito(codEv, numEd, idPart)
);

/**
 * Tabela escreve
 * @idAut, idArt  chave primária
 *
 * @idAut  número identificador do autor do artigo
 * @idArt  número identificador do artigo
 * 
 * @PK_ESCREVE   restrição de chave primária
 * @FK_ESCREVE1  restrição de chave estrangeira com a tabela pessoa
 * @FK_ESCREVE2  restrição de chave estrangeira com a tabela artigo
 */
CREATE TABLE escreve(
	idAut NUMBER(5) NOT NULL,
	idArt NUMBER(5) NOT NULL,
	CONSTRAINT PK_ESCREVE  PRIMARY KEY(idAut, idArt),
	CONSTRAINT FK_ESCREVE1 FOREIGN KEY(idAut) REFERENCES pessoa(idPe),
	CONSTRAINT FK_ESCREVE2 FOREIGN KEY(idArt) REFERENCES artigo(idArt)
);

/**
 * Tabela organiza
 * @idOrg, codEv, numEd  chave primária
 *
 * @idOrg     número identificador do organizador
 * @codEv     código do evento
 * @numEd     número da edição do evento
 * @cargoOrg  cargo do organizador
 *
 * @PK_ORGANIZA   restrição de chave primária
 * @FK_ORGANIZA1  restrição de chave estrangeira com a tabela pessoa
 * @FK_ORGANIZA2  restrição de chave estrangeira com a tabela edicao
 */
CREATE TABLE organiza(
	idOrg    NUMBER(5)     NOT NULL, 
	codEv    NUMBER(5)     NOT NULL, 
	numEd    NUMBER(5)     NOT NULL, 
	cargoOrg VARCHAR2(40),
	CONSTRAINT PK_ORGANIZA  PRIMARY KEY(idOrg, codEv, numEd),
	CONSTRAINT FK_ORGANIZA1 FOREIGN KEY(idOrg) REFERENCES pessoa(idPe),
	CONSTRAINT FK_ORGANIZA2 FOREIGN KEY(codEv, numEd) REFERENCES edicao(codEv, numEd)
);

/**
 * Tabela patrocinador
 * @cnpjPat  chave primária
 * 
 * @cnpjPat         CNPJ do patrocinador
 * @razaoSocialPat  razão social do patrocinador
 * @telefonePat     telefone do patrocinador
 * @enderecoPat     endereço do patrocinador
 *
 * @PK_PATROCINADOR  restrição de chave primária
 */
CREATE TABLE patrocinador(
	cnpjPat        VARCHAR2(30) NOT NULL,
	razaoSocialPat VARCHAR2(50), 
	telefonePat    VARCHAR2(15), 
	enderecoPat    VARCHAR2(100),
	CONSTRAINT PK_PATROCINADOR PRIMARY KEY(cnpjPat)
);

/**
 * Tabela patrocinio
 * @cnpjPat, codEv, numEd  chave primária
 * 
 * @cnpjPat   CNPJ do patrocinador
 * @codEv     código do evento
 * @numEd     número da edição do evento
 * @valorPat  valor do patrocínio
 * @saldoPat  saldo do patrocínio
 * @dataPat   data do patrocínio
 *
 * @PK_PATROCINIO   restrição de chave primária
 * @FK_PATROCINIO1  restrição de chave estrangeira com a tabela patrocinador
 * @FK_PATROCINIO2  restrição de chave estrangeira com a tabela edicao
 */
CREATE TABLE patrocinio(
	cnpjPat  VARCHAR2(30) NOT NULL, 
	codEv    NUMBER(5)    NOT NULL, 
	numEd    NUMBER(5)    NOT NULL, 
	valorPat NUMBER(9,2), 
	saldoPat NUMBER(9,2), 
	dataPat  DATE,
	CONSTRAINT PK_PATROCINIO  PRIMARY KEY(cnpjPat, codEv, numEd),
	CONSTRAINT FK_PATROCINIO1 FOREIGN KEY(cnpjPat) REFERENCES patrocinador(cnpjPat),
	CONSTRAINT FK_PATROCINIO2 FOREIGN KEY(codEv, numEd) REFERENCES edicao(codEv, numEd)
);

/**
 * Tabela despesa
 * @codDesp, codEv, numEd  chave primária
 * 
 * @codDesp        código da despesa
 * @codEv          código do evento
 * @numEd          número da edição do evento
 * @cnpjPat        CNPJ do patrocinador
 * @codEvPat       código do evento do patrocinador
 * @numEdPat       número da edição do evento do patrocinador
 * @dataDesp       data da despesa
 * @valorDesp      valor da despesa
 * @descricaoDesp  descrição da despesa
 *
 * @PK_DESPESA   restrição de chave primária
 * @FK_DESPESA1  restrição de chave primária com a tabela edicao
 * @FK_DESPESA2  restrição de chave primária com a tabela patrocinio
 */
CREATE TABLE despesa(
	codDesp       NUMBER(5)     NOT NULL, 
	codEv         NUMBER(5)     NOT NULL, 
	numEd         NUMBER(5)     NOT NULL, 
	cnpjPat       VARCHAR2(30), 
	codEvPat      NUMBER(5), 
	numEdPat      NUMBER(5), 
	dataDesp      DATE, 
	valorDesp     NUMBER(9,2),
	descricaoDesp VARCHAR2(100),
	CONSTRAINT PK_DESPESA  PRIMARY KEY(codDesp, codEv, numEd),
	CONSTRAINT FK_DESPESA1 FOREIGN KEY(codEv, numEd) REFERENCES edicao(codEv, numEd),
	CONSTRAINT FK_DESPESA2 FOREIGN KEY(cnpjPat, codEvPat, numEdPat) REFERENCES patrocinio(cnpjPat, codEv, numEd)
);

/**
 * Tabela auxilio
 * @codEvApr, numEdApr, idApr, tipoAux  chave primária
 *
 * @cnpjPat   CNPJ do patrocinador
 * @codEvPat  código do evento do patrocinador
 * @numEdPat  número da edição do evento do patrocinador
 * @codEvApr  código do evento do apresentador
 * @numEdApr  número da edição do evento do apresentador
 * @idApr     número de identificação do apresentador
 * @valorAux  valor do auxílio
 * @dataAux   data do auxílio
 * @tipoAux   tipo do auxílio
 *
 * @PK_AUXILIO   restrição de chave primária
 * @FK_AUXILIO1  restrição de chave estrangeira com a tabela patrocinio
 * @FK_AUXILIO2  restrição de chave estrangeira com a tabela inscrito
 * @CH_AUXILIO   chegagem do tipo do auxílio, que só pode ser hospedagem, alimentação ou transporte
 */
CREATE TABLE auxilio(
	cnpjPat  VARCHAR2(30), 
	codEvPat NUMBER(5), 
	numEdPat NUMBER(5), 
	codEvApr NUMBER(5)     NOT NULL, 
	numEdApr NUMBER(5)     NOT NULL, 
	idApr    NUMBER(5)     NOT NULL,
	valorAux NUMBER(9,2), 
	dataAux  DATE, 
	tipoAux  VARCHAR2(15)  NOT NULL, 
	CONSTRAINT PK_AUXILIO  PRIMARY KEY(codEvApr, numEdApr, idApr, tipoAux),
	CONSTRAINT FK_AUXILIO1 FOREIGN KEY(cnpjPat, codEvPat, numEdPat) REFERENCES patrocinio(cnpjPat, codEv, numEd),
	CONSTRAINT FK_AUXILIO2 FOREIGN KEY(codEvApr, numEdApr, idApr) REFERENCES inscrito(codEv, numEd, idPart),
	CONSTRAINT CH_AUXILIO  CHECK(tipoAux IN ('hospedagem', 'alimentação', 'transporte'))
);

/*
- O valor de qtdArtigosApresentados em cada edição foi constante de 15.
- O valor de totalArtigosApresentados no evento foi de 15 * 3 = 45

- De acordo com os dados inseridos, os valores deveriados para o saldoFinanceiroEd
foram calculados como se segue:

	inscricao(1, 1) = ( 90 * 30)
	inscricao(1, 2) = (110 * 30)
	inscricao(1, 3) = (110 * 30)

	patrocinio(1, 1) = 20000 + 19000 + 18000 + 17000
	patrocinio(1, 2) = 25000 + 24000 + 23000 + 22000
	patrocinio(1, 3) = 19000 + 18000 + 17000 + 16000

	despesas = (10000 + 7000 + 10000 + 6000 + 5000 + 10000 + 5000 + 10000)
	auxilios = (500 + 500 + 150 + 150 + 200 + 200 + 200 + 200 + 150 + 150)

	saldoFinanceiroEd(1, 1) = inscricao(1, 1)  + patrocinio(1, 1) - despesas - auxilios = 11300.00
	saldoFinanceiroEd(1, 2) = inscricao(1, 2)  + patrocinio(1, 2) - despesas - auxilios = 31900.00 
	saldoFinanceiroEd(1, 3) = inscricao(1, 3)  + patrocinio(1, 3) - despesas - auxilios =  7900.00
*/


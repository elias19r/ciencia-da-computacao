/**
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 * 
 * SCC0241 Laboratório de Bases de Dados
 * Turma 3
 * 
 * Trabalho Prático 4 - "scriptCriaInsere"
 * Script de Criação das Tabelas e Inserção de Dados
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
/
-- Drop em todas as sequências
DROP SEQUENCE SEQ_CODEV_EVENTO;
DROP SEQUENCE SEQ_NUMED_EDICAO;
DROP SEQUENCE SEQ_IDPE_PESSOA;
DROP SEQUENCE SEQ_IDART_ARTIGO;
DROP SEQUENCE SEQ_CODDESP_DESPESA;
/
-- Criação das sequencias
CREATE SEQUENCE SEQ_CODEV_EVENTO    MINVALUE 0 START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE SEQ_NUMED_EDICAO    MINVALUE 0 START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE SEQ_IDPE_PESSOA     MINVALUE 0 START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE SEQ_IDART_ARTIGO    MINVALUE 0 START WITH 1 INCREMENT BY 1;
CREATE SEQUENCE SEQ_CODDESP_DESPESA MINVALUE 0 START WITH 1 INCREMENT BY 1;
/
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
/
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
/
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
/
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
/
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
/
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
/
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
/
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
/
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
/
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
/
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
/
-- Inserção de dados nas tabelas
/
/**
 * Tabela evento(codEv, nomeEv, descricaoEv, websiteEv, totalArtigosApresentadosEv)
 * 
 * @codEv                       NUMBER(5)      NOT NULL
 * @nomeEv                      VARCHAR2(50)   NOT NULL
 * @descricaoEv                 VARCHAR2(300)
 * @websiteEv                   VARCHAR2(50)
 * @totalArtigosApresentadosEv  NUMBER(5)
 */
INSERT INTO evento VALUES(SEQ_CODEV_EVENTO.NEXTVAL, 'Simpósio de Tecnologia da Informação (STI)', 'O STI é um evento que ocorre anualmente no Brasil sobre tecnologia da informação reunindo acadêmicos e professionais técnicos da área.', 'www.sti-evento.org.br', 45);
/
/**
 * Tabela edicao(codEv, numEd, descricaoEd, dataInicioEd, dataFimEd, localEd, taxaEd, saldoFinanceiroEd, qtdArtigosApresentadosEd)
 * 
 * codEv                    NUMBER(5)      NOT NULL
 * numEd                    NUMBER(5)      NOT NULL
 * descricaoEd              VARCHAR2(300) 
 * dataInicioEd             DATE
 * dataFimEd                DATE
 * localEd                  VARCHAR2(50)
 * taxaEd                   NUMBER(4,2)
 * saldoFinanceiroEd        NUMBER(6,2)
 * qtdArtigosApresentadosEd NUMBER(5)
 */
INSERT INTO edicao VALUES(1, SEQ_NUMED_EDICAO.NEXTVAL, 'Edição piloto do evento STI: WWW e Telecomunicações.',              TO_DATE('01-06-2011', 'DD-MM-YYYY'), TO_DATE('05-06-2011', 'DD-MM-YYYY'), 'Hotel Morumbi - São Paulo, SP',            90.00, 11300.00,  15);
INSERT INTO edicao VALUES(1, SEQ_NUMED_EDICAO.NEXTVAL, 'Segunda edição do evento STI: Tecnologias Assistivas.',             TO_DATE('03-07-2012', 'DD-MM-YYYY'), TO_DATE('08-07-2012', 'DD-MM-YYYY'), 'Hotel Anacã - São Carlos, SP',            110.00, 31900.00 , 15);
INSERT INTO edicao VALUES(1, SEQ_NUMED_EDICAO.NEXTVAL, 'Terceira edição do evento STI: Web móvel e Redes Descentralizadas', TO_DATE('11-06-2013', 'DD-MM-YYYY'), TO_DATE('16-06-2013', 'DD-MM-YYYY'), 'Universidade de Brasília - Brasília, DF', 110.00,  7900.00,  15);
/
/**
 * Tabela pessoa(idPe, nomePe, emailPe, instituicaoPe, telefonePe, nacionalidadePe, enderecoPe, tipoOrganizador, tipoParticipante, tipoAutor)
 * 
 * idPe             NUMBER(5)     NOT NULL
 * nomePe           VARCHAR2(50)  NOT NULL
 * emailPe          VARCHAR2(50)  NOT NULL
 * instituicaoPe    VARCHAR2(50)
 * telefonePe       VARCHAR2(15)
 * nacionalidadePe  VARCHAR2(20)
 * enderecoPe       VARCHAR2(100)
 * tipoOrganizador  CHAR(1)
 * tipoParticipante CHAR(1)
 * tipoAutor        CHAR(1)
 */
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Bruno Burver',        'bruno.burver@usp.br',                       'Universidade de São Paulo',             '(16) 2332-3242',   'Brasileiro',   'Av. Sallum, 232',                 'O' , 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Helena Delgha',       'helena.delgha@gmail.com',                   'Universidade de São Paulo',             '(16) 9232-3284',   'Brasileiro',   'Av. São Carlos, 9229',            'O' , 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Caio Wara',           'caio.wara@gmail.com',                       'Universidade de São Paulo',             '(11) 4433-2234',   'Brasileiro',   'Av. Morades de Lourenço, 382',    'O' , 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Marcelo Henrique',    'marcelo.henrique@camps.br',                 'Universidade Estadual de Campinas',     '(18) 3242-9899',   'Brasileiro',   'Av. Coronel Salles, 372',         'O' , 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Antonio Ribeiro',     'antonio.ribeiro@camps.br',                  'Universidade Estadual de Campinas',     '(11) 2321-2344',   'Brasileiro',   'Av. Campo Grande, 22',            'O' , 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'João Silva',          'joao.silva@ifc.edu.br',                     'Instituto Federal de Computação',       '(11) 2341-2345',   'Brasileiro',   'Av. Joaquim Franciso, 123',       NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Hugo Silveira',       'hugo.silveira@ifc.edu.br',                  'Instituto Federal de Computação',       '(11) 9922-2121',   'Brasileiro',   'Av. Texeira de Barros, 45',       NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Rodrigo Schutzer',    'rodrigo.schutzer@ifc.edu.br',               'Instituto Federal de Computação',       '(11) 9927-4353',   'Brasileiro',   'Av. Fernando Pessoa, 97',         NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Frederico Mello',     'frederico.mello@ifc.edu.br',                'Instituto Federal de Computação',       '(11) 9898-1222',   'Brasileiro',   'Av. Portuguesa, 124',             NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Maria selva',         'maria.selva@ita.br',                        'Instituto Tecnológico de Alagoas',      '(34) 9983-8723',   'Brasileiro',   'Av. Dom Pedro, 989',              NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Anderson Lima',       'anderson.lima@ita.br',                      'Instituto Tecnológico de Alagoas',      '(34) 4334-1122',   'Brasileiro',   'Av. João Maria, 332',             NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'André Fagundes',      'andre.fagundes@ita.br',                     'Instituto Tecnológico de Alagoas',      '(34) 9992-2233',   'Brasileiro',   'Av. Aparecidinha, 14',            NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Júlia Antonieta',     'julia.antonieta@ita.br',                    'Instituto Tecnológico de Alagoas',      '(34) 2323-6644',   'Brasileiro',   'Av. Oblívio Mastelaro, 47',       NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Rodolfo Marcos',      'rodolfo.marcos@unisantos.br',               'Univerdade Federal de Santos',          '(21) 2938-4832',   'Brasileiro',   'Av. Tortorelli, 3321',            NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Luis Santos',         'luis.santos@unisantos.br',                  'Univerdade Federal de Santos',          '(21) 8374-4832',   'Brasileiro',   'Av. Boa Finança, 83',             NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Rafael Meneguelli',   'rafael.meneguelli@unisantos.br',            'Univerdade Federal de Santos',          '(21) 9091-0192',   'Brasileiro',   'Av. Jorge Amado, 88',             NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'José Jota',           'jose.jota@iab.edu.br',                      'Instituto Aeronáutico de Brasília',     '(15) 9092-1232',   'Brasileiro',   'Av. Quarenta, 212',               NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Sakura Katavi',       'sakura.katavi@iab.edu.br',                  'Instituto Aeronáutico de Brasília',     '(15) 9090-3489',   'Brasileiro',   'Av. Sexta, 10',                   NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Selene Rani',         'selene.rani@maensino.br',                   'Universidade Estadual Marcos Antonio',  '(47) 2313-2843',   'Brasileiro',   'Rua Penápolis Cardoso, 2131',     NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Raimundo Amaral',     'raimundo.amaral@maensino.br',               'Universidade Estadual Marcos Antonio',  '(47) 9281-9864',   'Brasileiro',   'Rua Curtiba, 231',                NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Yuri Silveira',       'yuri.silveira@maensino.br',                 'Universidade Estadual Marcos Antonio',  '(47) 2471-5614',   'Brasileiro',   'Rua Profa. Estrella Voltare, 11', NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Dartano Quei',        'dartano.quei@casadosoftware.com',           'Casa do Software',                      '(12) 3242-3232',   'Brasileiro',   'Rua São Cristovão, 454',          NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Orttano Quei',        'orttano.quei@casadosoftware.com',           'Casa do Software',                      '(12) 3242-3232',   'Brasileiro',   'Rua São Cristovão, 454',          NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Marcos Silvio',       'marcos.silvio@age.edu.br',                  'Amando Genso Ensio',                    '(23) 2347-1732',   'Brasileiro',   'Rua Republica Velha, 2345',       NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Jonas Antonio',       'jonas.antonio@age.edu.br',                  'Amando Genso Ensio',                    '(23) 2398-1932',   'Brasileiro',   'Rua Nove de Julho, 81',           NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Joseph Amadeu',       'joseph.amadeu@datainst.org',                'Data Institute',                        '(33) 2323-2314',   'Americano',    'Rua Aquidaban, 5993',             NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Adrian Brian',        'adrian.brian@datainst.org',                 'Data Institute',                        '(33) 2323-2314',   'Americano',    'Rua Aquidaban, 5993',             NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Anvesit Arwarser',    'anvesit.arwarser@google.com',               'Google Reasearch Institute',            '(11) 3748-5744',   'Americano',    'Av. Paulista, 234',               NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Isayo Streaw',        'isayo.streaw@google.com',                   'Google Reasearch Institute',            '(11) 3748-5744',   'Americano',    'Av. Paulista, 234',               NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Bandel Rakbel',       'bandel.rakbel@google.com',                  'Google Reasearch Institute',            '(11) 3748-5744',   'Americano',    'Av. Paulista, 234',               NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Regina Moreira',      'regina.moreira@ralles.com.br',              'Ralles Design',                         '(23) 2322-2311',   'Brasileiro',   'Av. José Bonifácio, 2922',        NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Osvaldo Rodrigues',   'osvaldo.rodrigues@ralles.com.br',           'Ralles Desing',                         '(16) 8872-2211',   'Brasileiro',   'Rua João Figueireido, 88',        NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Naril Urton',         'narial.urton@ukcs.co.uk',                   'UK Computer Science',                   '(45) 2387-9733',   'Inglês',       'Av. Lourenço Damiano, 998',       NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Onat Iathu',          'onat.iathu@ukcs.co.uk',                     'UK Computer Science',                   '(11) 2839-3222',   'Francês',      'Av. Repulicanos, 13',             NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Rodan Iron',          'rodan.iron@mtg.edu',                        'MTG University',                        '(11) 6669-9996',   'Americano',    'Av. Montanha Reluzente, 111',     NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Mendez Johnston',     'mendez.johnston@mtg.edu',                   'MTG University',                        '(11) 9239-2531',   'Americano',    'Av. Texeira de Barros, 22',       NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Roden Tinity',        'roden.tinity@outlook.com',                  'MSN Data Group',                        '(65) 2387-9847',   'Americano',    'Av. Paulista, 112',               NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Hinu Vescano',        'hinu.vescano@outlook.com',                  'MSN Data Group',                        '(65) 8293-2315',   'Mexicano',     'Av. Paulista, 112',               NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Joel Ondani',         'joel.ondani@ua.org',                        'Universidade Aberta',                   '(11) 2432-9873',   'Brasileiro',   'Rua Novo Horizonte, 5312',        NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Adams Vesmurte',      'adams.vesmurte@ua.org',                     'Universidade Aberta',                   '(33) 2292-2922',   'Brasileiro',   'Rua dos Curumins, 776',           NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Mario Kitano',        'mario.kitano@ua.org',                       'Universidade Aberta',                   '(15) 1192-2939',   'Brasileiro',   'Rua Padre Arsênio, 20',           NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Manoel Joaquim',      'manoel.joaquim@novaera.edu.br',             'Universidade Nova Era',                 '(87) 2737-1493',   'Brasileiro',   'Av. Dom João X, 431',             NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Silvio Martines',     'silvio.martines@novaera.edu.br',            'Universidade Nova Era',                 '(11) 3829-2321',   'Brasileiro',   'Rua dos Marajás, 727',            NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Matheus Yolo',        'matheus.yolo@instnacp.edu.br',              'Instituto Nacional de Pesquisa',        '(21) 3672-5346',   'Brasileiro',   'Av. Maracanã Pompeu, 2336',       NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Cristina Fernandes',  'cristina.fernandes@instnacp.edu.br',        'Instituto Nacional de Pesquisa',        '(11) 8293-2382',   'Brasileiro',   'Av. José Moraes, 1213',           NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Gretchen Hermano',    'gretchen.hermano@registro.br',              'Registro.br',                           '(11) 8723-1121',   'Brasileiro',   'Av. Marcondes Matos, 21',         NULL, 'P',  NULL);
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Rosana Aparecida',    'rosana.aparecida@registro.br',              'Registro.br',                           '(33) 3211-5423',   'Brasileiro',   'Av. José Bonifácio, 101',         NULL, 'P',  NULL);
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Júlio Cesar',         'julio.cesar@registro.br',                   'Registro.br',                           '(19) 9981-8433',   'Brasileiro',   'Rua Marco Lucas, 920',            NULL, 'P',  NULL);
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Vera Lúcia',          'vera.lucia@htech.com.br',                   'HTech Soluções',                        '(19) 8723-0909',   'Brasileiro',   'Av. Dr. Alfredo, 9',              NULL, 'P',  NULL);
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Maria Lombardi',      'maria.lombardi@htech.com.br',               'HTech Soluções',                        '(19) 1928-9772',   'Brasileiro',   'Av. Álvaro Macedo, 233',          NULL, 'P',  NULL);
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Manuel Alcântara',    'manuel.alcantara@htech.com.br',             'HTech Soluções',                        '(19) 8181-8120',   'Brasileiro',   'Rua Filomena, 12',                NULL, 'P',  NULL);
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Amadeu Marques',      'amadeu.marques@uniriopreto.br',             'Universidade de Rio Preto',             '(19) 7382-8473',   'Brasileiro',   'Av. Martins da Rocha, 874',       NULL, 'P',  NULL);
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Milena Matias',       'milena.matias@uniriopreto.br',              'Universidade de Rio Preto',             '(19) 2312-0234',   'Brasileiro',   'Av. Manuela Azevedo, 846',        NULL, 'P',  NULL);
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Robson Silva',        'robson.silva@uniriopreto.br',               'Universidade de Rio Preto',             '(19) 2211-8377',   'Brasileiro',   'Av. Carlinhos Fraga, 112',        NULL, 'P',  NULL);
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Rudinei Patrício',    'rudinei.patricio@uniriopreto.br',           'Universidade de Rio Preto',             '(19) 5655-4433',   'Brasileiro',   'Av. Prof. Augusto Ponte, 15',     NULL, 'P',  NULL);
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Arlindo Rodrigues',   'arlindo.rodrigues@hpp.com.br',              'HP Power Inc.',                         '(11) 3974-6566',   'Brasileiro',   'Av. Vinícuis de Moares, 454',     NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Sônia Feliciano',     'sonia.feliciano@hpp.com.br',                'HP Power Inc.',                         '(11) 2314-5141',   'Brasileiro',   'Rua Prof. Machado Rosa, 434',     NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Ricardo Maia',        'ricardo.maia@hpp.com.br',                   'HP Power Inc.',                         '(11) 9874-4315',   'Brasileiro',   'Rua Antonio Bandeira, 122',       NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Beatriz Rubens',      'beatriz.rubens@hpp.com.br',                 'HP Power Inc.',                         '(11) 8576-6563',   'Brasileiro',   'Av. Costa Rica, 111',             NULL, 'P',  'A' );
INSERT INTO pessoa VALUES(SEQ_IDPE_PESSOA.NEXTVAL, 'Andrei Virtuzo',      'andrei.virtuzo@hpp.com.br',                 'HP Power Inc.',                         '(11) 2344-5511',   'Brasileiro',   'Rua Juliano Neto, 100',           NULL, 'P',  'A' );
/
/**
 * Tabela inscrito(codEv, numEd, idPart, dataInsc, tipoApresentador)
 * 
 * codEv            NUMBER(5)  NOT NULL 
 * numEd            NUMBER(5)  NOT NULL 
 * idPart           NUMBER(5)  NOT NULL
 * dataInsc         DATE
 * tipoApresentador CHAR(1)
 */
INSERT INTO inscrito VALUES(1, 1, 1,  TO_DATE('01-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 2,  TO_DATE('01-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 3,  TO_DATE('01-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 4,  TO_DATE('01-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 5,  TO_DATE('02-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 6,  TO_DATE('02-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 7,  TO_DATE('02-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 8,  TO_DATE('02-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 9,  TO_DATE('02-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 10, TO_DATE('02-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 11, TO_DATE('02-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 12, TO_DATE('03-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 13, TO_DATE('03-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 14, TO_DATE('03-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 15, TO_DATE('03-05-2011', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 1, 16, TO_DATE('03-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 17, TO_DATE('03-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 18, TO_DATE('03-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 19, TO_DATE('03-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 20, TO_DATE('03-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 21, TO_DATE('04-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 22, TO_DATE('04-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 23, TO_DATE('04-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 24, TO_DATE('04-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 25, TO_DATE('04-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 26, TO_DATE('04-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 27, TO_DATE('10-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 28, TO_DATE('12-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 29, TO_DATE('20-05-2011', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 1, 30, TO_DATE('20-05-2011', 'DD-MM-YYYY'), NULL);
/
INSERT INTO inscrito VALUES(1, 2, 1,  TO_DATE('02-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 2,  TO_DATE('02-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 3,  TO_DATE('02-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 4,  TO_DATE('05-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 5,  TO_DATE('05-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 6,  TO_DATE('05-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 7,  TO_DATE('05-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 8,  TO_DATE('05-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 9,  TO_DATE('07-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 10, TO_DATE('07-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 41, TO_DATE('07-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 42, TO_DATE('07-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 43, TO_DATE('09-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 44, TO_DATE('09-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 45, TO_DATE('09-05-2012', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 2, 46, TO_DATE('09-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 47, TO_DATE('09-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 48, TO_DATE('09-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 49, TO_DATE('10-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 50, TO_DATE('10-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 51, TO_DATE('10-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 52, TO_DATE('10-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 53, TO_DATE('11-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 54, TO_DATE('11-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 55, TO_DATE('12-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 56, TO_DATE('13-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 57, TO_DATE('13-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 58, TO_DATE('13-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 59, TO_DATE('22-05-2012', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 2, 60, TO_DATE('22-05-2012', 'DD-MM-YYYY'), NULL);
/
INSERT INTO inscrito VALUES(1, 3, 1,  TO_DATE('17-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 2,  TO_DATE('17-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 3,  TO_DATE('17-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 4,  TO_DATE('17-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 5,  TO_DATE('17-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 6,  TO_DATE('17-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 7,  TO_DATE('17-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 8,  TO_DATE('17-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 9,  TO_DATE('18-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 10, TO_DATE('18-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 21, TO_DATE('18-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 22, TO_DATE('18-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 23, TO_DATE('18-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 24, TO_DATE('18-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 25, TO_DATE('18-05-2013', 'DD-MM-YYYY'), 'S' );
INSERT INTO inscrito VALUES(1, 3, 26, TO_DATE('18-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 27, TO_DATE('20-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 28, TO_DATE('20-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 29, TO_DATE('20-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 30, TO_DATE('20-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 31, TO_DATE('20-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 32, TO_DATE('20-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 33, TO_DATE('21-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 34, TO_DATE('21-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 35, TO_DATE('21-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 36, TO_DATE('21-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 37, TO_DATE('21-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 38, TO_DATE('21-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 39, TO_DATE('21-05-2013', 'DD-MM-YYYY'), NULL);
INSERT INTO inscrito VALUES(1, 3, 40, TO_DATE('21-05-2013', 'DD-MM-YYYY'), NULL);
/
/**
 * Tabela artigo(idArt, tituloArt, dataApresArt, horaApresArt, codEv, numEd, idApr)
 * 
 * idArt        NUMBER(5)      NOT NULL
 * tituloArt    VARCHAR2(100)  NOT NULL
 * dataApresArt DATE
 * horaApresArt TIMESTAMP
 * codEv        NUMBER(5) 
 * numEd        NUMBER(5) 
 * idApr        NUMBER(5)
 */
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Análise de Dados na Web 2.0',                                      TO_DATE('01-06-2011', 'DD-MM-YYYY'),  TO_DATE('01-06-2011 09:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 1 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Apresentação de novas tecnologias de e-mail',                      TO_DATE('01-06-2011', 'DD-MM-YYYY'),  TO_DATE('01-06-2011 11:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 2 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Mudanças na nova geração de usuário de Internet',                  TO_DATE('01-06-2011', 'DD-MM-YYYY'),  TO_DATE('01-06-2011 15:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 3 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Proposta de Protocolos Seguros para TCP/IP',                       TO_DATE('01-06-2011', 'DD-MM-YYYY'),  TO_DATE('01-06-2011 17:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 4 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Estudo de Caso: Hacking',                                          TO_DATE('02-06-2011', 'DD-MM-YYYY'),  TO_DATE('02-06-2011 09:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 5 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'HTML5 e suas novidades',                                           TO_DATE('02-06-2011', 'DD-MM-YYYY'),  TO_DATE('02-06-2011 11:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 6 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Uma aborgagem sobre o uso de dados públicos',                      TO_DATE('02-06-2011', 'DD-MM-YYYY'),  TO_DATE('02-06-2011 15:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 7 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Estudo de Caso: Um navegador "from scratch',                       TO_DATE('03-06-2011', 'DD-MM-YYYY'),  TO_DATE('03-06-2011 09:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 8 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Computação e Sociedade: O impacto da redes sociais',               TO_DATE('03-06-2011', 'DD-MM-YYYY'),  TO_DATE('03-06-2011 11:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 9 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Análise do crescimento de acesso banda larga no Brasil',           TO_DATE('03-06-2011', 'DD-MM-YYYY'),  TO_DATE('03-06-2011 15:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 10);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Mineração de Dados do Gorveno Brasileiro',                         TO_DATE('04-06-2011', 'DD-MM-YYYY'),  TO_DATE('04-06-2011 09:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 11);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'An Introduction to New Web Medias',                                TO_DATE('04-06-2011', 'DD-MM-YYYY'),  TO_DATE('04-06-2011 11:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 12);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Análise Comportalmente de Usuários',                               TO_DATE('04-06-2011', 'DD-MM-YYYY'),  TO_DATE('04-06-2011 15:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 13);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Banco de Dados e a Web 2.0: demanda vs recursos',                  TO_DATE('04-06-2011', 'DD-MM-YYYY'),  TO_DATE('04-06-2011 17:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 14);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Novos protótipos para Android',                                    TO_DATE('05-06-2011', 'DD-MM-YYYY'),  TO_DATE('05-06-2011 09:00', 'DD-MM-YYYY HH24:MI'), 1, 1, 15);
/
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Acessibilidade e HTML5',                                           TO_DATE('03-07-2012', 'DD-MM-YYYY'),  TO_DATE('03-07-2012 09:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 1 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Monitoramente de Tráfego em Redes Públicas',                       TO_DATE('03-07-2012', 'DD-MM-YYYY'),  TO_DATE('03-07-2012 11:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 2 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Gerenciamento de Dados Abertos',                                   TO_DATE('03-07-2012', 'DD-MM-YYYY'),  TO_DATE('03-07-2012 15:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 3 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Protocolos de Transferência de Dados Assistidas',                  TO_DATE('03-07-2012', 'DD-MM-YYYY'),  TO_DATE('03-07-2012 17:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 4 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Estudo de Caso: Navegando na Web com Deficiência Visual',          TO_DATE('04-07-2012', 'DD-MM-YYYY'),  TO_DATE('04-07-2012 09:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 5 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Uma Introdução ao Orca',                                           TO_DATE('04-07-2012', 'DD-MM-YYYY'),  TO_DATE('04-07-2012 11:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 6 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Técnicas para Modelagem de Dados voltadas para a acessibilidade',  TO_DATE('04-07-2012', 'DD-MM-YYYY'),  TO_DATE('04-07-2012 15:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 7 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, '10 passos para tornar seu site acessível',                         TO_DATE('05-07-2012', 'DD-MM-YYYY'),  TO_DATE('05-07-2012 09:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 8 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Congestionamento de Redes e DoS',                                  TO_DATE('05-07-2012', 'DD-MM-YYYY'),  TO_DATE('05-07-2012 15:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 9 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'A Limitação de Banda Larga no Brasil',                             TO_DATE('06-07-2012', 'DD-MM-YYYY'),  TO_DATE('06-07-2012 09:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 10);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Análise da Qualidade de Conexão Móvel no Brasil',                  TO_DATE('06-07-2012', 'DD-MM-YYYY'),  TO_DATE('06-07-2012 15:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 41);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Gestão de Energia em Serviços Críticos',                           TO_DATE('07-07-2012', 'DD-MM-YYYY'),  TO_DATE('07-07-2012 09:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 42);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'An Introduction to the Web of Things',                             TO_DATE('07-07-2012', 'DD-MM-YYYY'),  TO_DATE('07-07-2012 15:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 43);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Os principais marcos da mobilidade da Web e seus impactos',        TO_DATE('08-07-2012', 'DD-MM-YYYY'),  TO_DATE('08-07-2012 09:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 44);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Sugestão de Protótipo para Operação de Computadores',              TO_DATE('08-07-2012', 'DD-MM-YYYY'),  TO_DATE('08-07-2012 15:00', 'DD-MM-YYYY HH24:MI'), 1, 2, 45);
/
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'A Descentralização da Internet e o que Você tem a Ver com Isso',   TO_DATE('11-06-2013', 'DD-MM-YYYY'),  TO_DATE('11-06-2013 09:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 1 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Alternativas para Redes Sociais Descentralizadas',                 TO_DATE('11-06-2013', 'DD-MM-YYYY'),  TO_DATE('11-06-2013 11:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 2 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Criptografia Atual',                                               TO_DATE('11-06-2013', 'DD-MM-YYYY'),  TO_DATE('11-06-2013 15:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 3 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Marco Civil da Internet',                                          TO_DATE('11-06-2013', 'DD-MM-YYYY'),  TO_DATE('11-06-2013 17:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 4 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Descentralização Monetária: A "Revolução" do Bitcoin',             TO_DATE('12-06-2013', 'DD-MM-YYYY'),  TO_DATE('12-06-2013 09:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 5 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Os 10 Mandamentos da Descentralização',                            TO_DATE('12-06-2013', 'DD-MM-YYYY'),  TO_DATE('12-06-2013 11:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 6 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Partipação Direita na Democracia, é possível?',                    TO_DATE('12-06-2013', 'DD-MM-YYYY'),  TO_DATE('12-06-2013 15:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 7 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Tecnologias de Uso Diário para Redes Móveis',                      TO_DATE('13-06-2013', 'DD-MM-YYYY'),  TO_DATE('13-06-2013 09:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 8 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'A Qualidade das Redes Móveis no Brasil',                           TO_DATE('13-06-2013', 'DD-MM-YYYY'),  TO_DATE('13-06-2013 15:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 9 );
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Cloud Computing, será?',                                           TO_DATE('14-06-2013', 'DD-MM-YYYY'),  TO_DATE('14-06-2013 09:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 10);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Distruição de Dados em Redes P2P',                                 TO_DATE('14-06-2013', 'DD-MM-YYYY'),  TO_DATE('14-06-2013 15:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 21);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Estudo de Caso: Criptografia nos sistemas bancários',              TO_DATE('15-06-2013', 'DD-MM-YYYY'),  TO_DATE('15-06-2013 09:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 22);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Kernel Linux for Distributed Systems',                             TO_DATE('15-06-2013', 'DD-MM-YYYY'),  TO_DATE('15-06-2013 15:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 23);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Análise: Os grandes datacenter e os recursos utilizados',          TO_DATE('16-06-2013', 'DD-MM-YYYY'),  TO_DATE('16-06-2013 09:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 24);
INSERT INTO artigo VALUES(SEQ_IDART_ARTIGO.NEXTVAL, 'Mineração de Dados em uma rede descentralizada',                   TO_DATE('16-06-2013', 'DD-MM-YYYY'),  TO_DATE('16-06-2013 15:00', 'DD-MM-YYYY HH24:MI'), 1, 3, 25);
/
/**
 * Tabela escreve(idAut, idArt)
 * 
 * idAut  NUMBER(5)  NOT NULL
 * idArt  NUMBER(5)  NOT NULL
 */
INSERT INTO escreve VALUES(1,  1 );
INSERT INTO escreve VALUES(2,  1 );
INSERT INTO escreve VALUES(3,  2 );
INSERT INTO escreve VALUES(4,  2 );
INSERT INTO escreve VALUES(5,  3 );
INSERT INTO escreve VALUES(6,  3 );
INSERT INTO escreve VALUES(7,  4 );
INSERT INTO escreve VALUES(8,  4 );
INSERT INTO escreve VALUES(9,  5 );
INSERT INTO escreve VALUES(10, 5 );
INSERT INTO escreve VALUES(11, 6 );
INSERT INTO escreve VALUES(12, 6 );
INSERT INTO escreve VALUES(13, 7 );
INSERT INTO escreve VALUES(14, 7 );
INSERT INTO escreve VALUES(15, 8 );
INSERT INTO escreve VALUES(16, 8 );
INSERT INTO escreve VALUES(17, 9 );
INSERT INTO escreve VALUES(18, 9 );
INSERT INTO escreve VALUES(19, 10);
INSERT INTO escreve VALUES(20, 10);
INSERT INTO escreve VALUES(21, 11);
INSERT INTO escreve VALUES(22, 11);
INSERT INTO escreve VALUES(23, 12);
INSERT INTO escreve VALUES(24, 12);
INSERT INTO escreve VALUES(25, 13);
INSERT INTO escreve VALUES(26, 13);
INSERT INTO escreve VALUES(27, 14);
INSERT INTO escreve VALUES(28, 14);
INSERT INTO escreve VALUES(29, 15);
INSERT INTO escreve VALUES(30, 15);
INSERT INTO escreve VALUES(31, 16);
INSERT INTO escreve VALUES(32, 16);
INSERT INTO escreve VALUES(33, 17);
INSERT INTO escreve VALUES(34, 17);
INSERT INTO escreve VALUES(35, 18);
INSERT INTO escreve VALUES(36, 18);
INSERT INTO escreve VALUES(37, 19);
INSERT INTO escreve VALUES(38, 19);
INSERT INTO escreve VALUES(39, 20);
INSERT INTO escreve VALUES(40, 20);
INSERT INTO escreve VALUES(41, 21);
INSERT INTO escreve VALUES(42, 21);
INSERT INTO escreve VALUES(43, 22);
INSERT INTO escreve VALUES(44, 22);
INSERT INTO escreve VALUES(45, 23);
INSERT INTO escreve VALUES(46, 23);
INSERT INTO escreve VALUES(47, 24);
INSERT INTO escreve VALUES(48, 24);
INSERT INTO escreve VALUES(49, 25);
INSERT INTO escreve VALUES(50, 25);
INSERT INTO escreve VALUES(51, 26);
INSERT INTO escreve VALUES(52, 26);
INSERT INTO escreve VALUES(53, 27);
INSERT INTO escreve VALUES(54, 27);
INSERT INTO escreve VALUES(55, 28);
INSERT INTO escreve VALUES(56, 28);
INSERT INTO escreve VALUES(57, 29);
INSERT INTO escreve VALUES(58, 29);
INSERT INTO escreve VALUES(59, 30);
INSERT INTO escreve VALUES(60, 30);
INSERT INTO escreve VALUES(1,  31);
INSERT INTO escreve VALUES(2,  31);
INSERT INTO escreve VALUES(3,  32);
INSERT INTO escreve VALUES(4,  32);
INSERT INTO escreve VALUES(5,  33);
INSERT INTO escreve VALUES(6,  33);
INSERT INTO escreve VALUES(7,  34);
INSERT INTO escreve VALUES(8,  34);
INSERT INTO escreve VALUES(9,  35);
INSERT INTO escreve VALUES(10, 35);
INSERT INTO escreve VALUES(11, 36);
INSERT INTO escreve VALUES(12, 36);
INSERT INTO escreve VALUES(13, 37);
INSERT INTO escreve VALUES(14, 37);
INSERT INTO escreve VALUES(15, 38);
INSERT INTO escreve VALUES(16, 38);
INSERT INTO escreve VALUES(17, 39);
INSERT INTO escreve VALUES(18, 39);
INSERT INTO escreve VALUES(19, 40);
INSERT INTO escreve VALUES(20, 40);
INSERT INTO escreve VALUES(21, 41);
INSERT INTO escreve VALUES(22, 41);
INSERT INTO escreve VALUES(23, 42);
INSERT INTO escreve VALUES(24, 42);
INSERT INTO escreve VALUES(25, 43);
INSERT INTO escreve VALUES(26, 43);
INSERT INTO escreve VALUES(27, 44);
INSERT INTO escreve VALUES(28, 44);
INSERT INTO escreve VALUES(29, 45);
INSERT INTO escreve VALUES(30, 45);
/
/**
 * Tabela organiza(idOrg, codEv, numEd, cargoOrg)
 * 
 * idOrg     NUMBER(5)     NOT NULL
 * codEv     NUMBER(5)     NOT NULL
 * numEd     NUMBER(5)     NOT NULL
 * cargoOrg  VARCHAR2(40)
 */
INSERT INTO organiza VALUES(1, 1, 1, 'Gerente');
INSERT INTO organiza VALUES(2, 1, 1, 'Organização');
INSERT INTO organiza VALUES(3, 1, 1, 'Agendamento de Apresentações');
INSERT INTO organiza VALUES(4, 1, 1, 'Financeiro');
INSERT INTO organiza VALUES(5, 1, 1, 'Divulgação');
/
INSERT INTO organiza VALUES(1, 1, 2, 'Gerente');
INSERT INTO organiza VALUES(2, 1, 2, 'Organização');
INSERT INTO organiza VALUES(3, 1, 2, 'Agendamento de Apresentações');
INSERT INTO organiza VALUES(4, 1, 2, 'Financeiro');
INSERT INTO organiza VALUES(5, 1, 2, 'Divulgação');
/
INSERT INTO organiza VALUES(1, 1, 3, 'Gerente');
INSERT INTO organiza VALUES(2, 1, 3, 'Organização');
INSERT INTO organiza VALUES(3, 1, 3, 'Agendamento de Apresentações');
INSERT INTO organiza VALUES(4, 1, 3, 'Financeiro');
INSERT INTO organiza VALUES(5, 1, 3, 'Divulgação');
/
/**
 * Tabela patrocinador(cnpjPat, razaoSocialPat, telefonePat, enderecoPat)
 * 
 * cnpjPat         VARCHAR2(30)  NOT NULL
 * razaoSocialPat  VARCHAR2(50) 
 * telefonePat     VARCHAR2(15) 
 * enderecoPat     VARCHAR2(100)
 */
INSERT INTO patrocinador VALUES('12.435.213/0221-10', 'IBM Brasil',                    '(11) 3221-2222', 'Av. Paulista, 23');
INSERT INTO patrocinador VALUES('23.543.231/0211-02', 'Itautec',                       '(11) 1123-2224', 'Av. Marcos Vias, 65');
INSERT INTO patrocinador VALUES('66.232.656/0111-03', 'SysDesign',                     '(11) 5454-5521', 'Av. Morumbi, 921');
INSERT INTO patrocinador VALUES('12.878.232/0131-28', 'Fundação de Amparo a Pesquisa', '(11) 7483-2378', 'Av. Dr. Fernão Pessoa, 321');
/
/**
 * Tabela patrocinio(cnpjPat, codEv, numEd, valorPat, saldoPat, dataPat)
 * 
 * cnpjPat   VARCHAR2(30)  NOT NULL
 * codEv     NUMBER(5)     NOT NULL
 * numEd     NUMBER(5)     NOT NULL
 * valorPat  NUMBER(4,2)
 * saldoPat  NUMBER(4,2)
 * dataPat   DATE
 */
INSERT INTO patrocinio VALUES('12.435.213/0221-10', 1, 1, 20000.00, 1700.00, TO_DATE('01-02-2011', 'DD-MM-YYYY'));
INSERT INTO patrocinio VALUES('23.543.231/0211-02', 1, 1, 19000.00, 2600.00, TO_DATE('11-02-2011', 'DD-MM-YYYY'));
INSERT INTO patrocinio VALUES('66.232.656/0111-03', 1, 1, 18000.00, 2600.00, TO_DATE('20-02-2011', 'DD-MM-YYYY'));
INSERT INTO patrocinio VALUES('12.878.232/0131-28', 1, 1, 17000.00, 1700.00, TO_DATE('25-02-2011', 'DD-MM-YYYY'));
/
INSERT INTO patrocinio VALUES('12.435.213/0221-10', 1, 2, 25000.00, 6700.00, TO_DATE('07-01-2012', 'DD-MM-YYYY'));
INSERT INTO patrocinio VALUES('23.543.231/0211-02', 1, 2, 24000.00, 7600.00, TO_DATE('12-01-2012', 'DD-MM-YYYY'));
INSERT INTO patrocinio VALUES('66.232.656/0111-03', 1, 2, 23000.00, 7600.00, TO_DATE('05-01-2012', 'DD-MM-YYYY'));
INSERT INTO patrocinio VALUES('12.878.232/0131-28', 1, 2, 22000.00, 6700.00, TO_DATE('09-01-2012', 'DD-MM-YYYY'));
/
INSERT INTO patrocinio VALUES('12.435.213/0221-10', 1, 3, 19000.00,  700.00, TO_DATE('11-02-2013', 'DD-MM-YYYY'));
INSERT INTO patrocinio VALUES('23.543.231/0211-02', 1, 3, 18000.00, 1600.00, TO_DATE('11-02-2013', 'DD-MM-YYYY'));
INSERT INTO patrocinio VALUES('66.232.656/0111-03', 1, 3, 17000.00, 1600.00, TO_DATE('13-02-2013', 'DD-MM-YYYY'));
INSERT INTO patrocinio VALUES('12.878.232/0131-28', 1, 3, 16000.00,  700.00, TO_DATE('15-02-2013', 'DD-MM-YYYY'));
/
/**
 * Tabela despesa(codDesp, codEv, numEd, cnpjPat, codEvPat, numEdPat, dataDesp, valorDesp, descricaoDesp)
 * 
 * codDesp        NUMBER(5)     NOT NULL
 * codEv          NUMBER(5)     NOT NULL
 * numEd          NUMBER(5)     NOT NULL
 * cnpjPat        VARCHAR2(30)
 * codEvPat       NUMBER(5)
 * numEdPat       NUMBER(5)
 * dataDesp       DATE
 * valorDesp      NUMBER(4,2)
 * descricaoDesp  VARCHAR2(100)
 */
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 1, '12.435.213/0221-10', 1, 1, TO_DATE('01-04-2011', 'DD-MM-YYYY'), 10000.00, 'Divulgação online');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 1, '12.435.213/0221-10', 1, 1, TO_DATE('02-04-2011', 'DD-MM-YYYY'),  7000.00, 'Coffee Break');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 1, '23.543.231/0211-02', 1, 1, TO_DATE('03-05-2011', 'DD-MM-YYYY'), 10000.00, 'Aluguel do espaço');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 1, '23.543.231/0211-02', 1, 1, TO_DATE('03-05-2011', 'DD-MM-YYYY'),  6000.00, 'Coquetel');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 1, '66.232.656/0111-03', 1, 1, TO_DATE('05-05-2011', 'DD-MM-YYYY'),  5000.00, 'Material de apresentação');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 1, '66.232.656/0111-03', 1, 1, TO_DATE('05-05-2011', 'DD-MM-YYYY'), 10000.00, 'Manutenção preventiva de equipamentos');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 1, '12.878.232/0131-28', 1, 1, TO_DATE('06-05-2011', 'DD-MM-YYYY'),  5000.00, 'Sistema de Inscrição');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 1, '12.878.232/0131-28', 1, 1, TO_DATE('20-05-2011', 'DD-MM-YYYY'), 10000.00, 'Conexão a Internet e energia');
/
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 2, '12.435.213/0221-10', 1, 2, TO_DATE('10-03-2012', 'DD-MM-YYYY'), 10000.00, 'Divulgação online');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 2, '12.435.213/0221-10', 1, 2, TO_DATE('14-03-2012', 'DD-MM-YYYY'),  7000.00, 'Coffee Break');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 2, '23.543.231/0211-02', 1, 2, TO_DATE('03-04-2012', 'DD-MM-YYYY'), 10000.00, 'Aluguel do espaço');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 2, '23.543.231/0211-02', 1, 2, TO_DATE('04-04-2012', 'DD-MM-YYYY'),  6000.00, 'Coquetel');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 2, '66.232.656/0111-03', 1, 2, TO_DATE('10-05-2012', 'DD-MM-YYYY'),  5000.00, 'Material de apresentação');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 2, '66.232.656/0111-03', 1, 2, TO_DATE('20-05-2012', 'DD-MM-YYYY'), 10000.00, 'Manutenção preventiva de equipamentos');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 2, '12.878.232/0131-28', 1, 2, TO_DATE('21-05-2012', 'DD-MM-YYYY'),  5000.00, 'Sistema de Inscrição');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 2, '12.878.232/0131-28', 1, 2, TO_DATE('22-05-2012', 'DD-MM-YYYY'), 10000.00, 'Conexão a Internet e energia');
/
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 3, '12.435.213/0221-10', 1, 3, TO_DATE('24-03-2013', 'DD-MM-YYYY'), 10000.00, 'Divulgação online');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 3, '12.435.213/0221-10', 1, 3, TO_DATE('24-03-2013', 'DD-MM-YYYY'),  7000.00, 'Coffee Break');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 3, '23.543.231/0211-02', 1, 3, TO_DATE('03-04-2013', 'DD-MM-YYYY'), 10000.00, 'Aluguel do espaço');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 3, '23.543.231/0211-02', 1, 3, TO_DATE('04-04-2013', 'DD-MM-YYYY'),  6000.00, 'Coquetel');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 3, '66.232.656/0111-03', 1, 3, TO_DATE('01-05-2013', 'DD-MM-YYYY'),  5000.00, 'Material de apresentação');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 3, '66.232.656/0111-03', 1, 3, TO_DATE('10-05-2013', 'DD-MM-YYYY'), 10000.00, 'Manutenção preventiva de equipamentos');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 3, '12.878.232/0131-28', 1, 3, TO_DATE('21-05-2013', 'DD-MM-YYYY'),  5000.00, 'Sistema de Inscrição');
INSERT INTO despesa VALUES(SEQ_CODDESP_DESPESA.NEXTVAL,  1, 3, '12.878.232/0131-28', 1, 3, TO_DATE('29-05-2013', 'DD-MM-YYYY'), 10000.00, 'Conexão a Internet e energia');
/
/**
 * Tabela auxilio(cnpjPat, codEvPat, numEdPat, codEvApr, numEdApr, idApr, valorAux, dataAux, tipoAux)
 * 
 * cnpjPat  VARCHAR2(30) 
 * codEvPat NUMBER(5) 
 * numEdPat NUMBER(5) 
 * codEvApr NUMBER(5)     NOT NULL
 * numEdApr NUMBER(5)     NOT NULL
 * idApr    NUMBER(5)     NOT NULL
 * valorAux NUMBER(4,2) 
 * dataAux  DATE 
 * tipoAux  VARCHAR2(15)  NOT NULL
 */
INSERT INTO auxilio VALUES('12.435.213/0221-10', 1, 1, 1, 1, 1 , 500.00, TO_DATE('01-06-2011', 'DD-MM-YYYY'), 'hospedagem' );
INSERT INTO auxilio VALUES('12.435.213/0221-10', 1, 1, 1, 1, 2 , 500.00, TO_DATE('01-06-2011', 'DD-MM-YYYY'), 'hospedagem' );
INSERT INTO auxilio VALUES('12.435.213/0221-10', 1, 1, 1, 1, 3 , 150.00, TO_DATE('01-06-2011', 'DD-MM-YYYY'), 'alimentação');
INSERT INTO auxilio VALUES('12.435.213/0221-10', 1, 1, 1, 1, 4 , 150.00, TO_DATE('01-06-2011', 'DD-MM-YYYY'), 'alimentação');
INSERT INTO auxilio VALUES('23.543.231/0211-02', 1, 1, 1, 1, 5 , 200.00, TO_DATE('02-06-2011', 'DD-MM-YYYY'), 'transporte' );
INSERT INTO auxilio VALUES('23.543.231/0211-02', 1, 1, 1, 1, 6 , 200.00, TO_DATE('02-06-2011', 'DD-MM-YYYY'), 'transporte' );
INSERT INTO auxilio VALUES('66.232.656/0111-03', 1, 1, 1, 1, 7 , 200.00, TO_DATE('03-06-2011', 'DD-MM-YYYY'), 'transporte' );
INSERT INTO auxilio VALUES('66.232.656/0111-03', 1, 1, 1, 1, 8 , 200.00, TO_DATE('03-06-2011', 'DD-MM-YYYY'), 'transporte' );
INSERT INTO auxilio VALUES('12.878.232/0131-28', 1, 1, 1, 1, 9 , 150.00, TO_DATE('04-06-2011', 'DD-MM-YYYY'), 'alimentação');
INSERT INTO auxilio VALUES('12.878.232/0131-28', 1, 1, 1, 1, 10, 150.00, TO_DATE('04-06-2011', 'DD-MM-YYYY'), 'alimentação');
/
INSERT INTO auxilio VALUES('12.435.213/0221-10', 1, 2, 1, 2, 1 , 500.00, TO_DATE('03-07-2012', 'DD-MM-YYYY'), 'hospedagem' );
INSERT INTO auxilio VALUES('12.435.213/0221-10', 1, 2, 1, 2, 2 , 500.00, TO_DATE('03-07-2012', 'DD-MM-YYYY'), 'hospedagem' );
INSERT INTO auxilio VALUES('12.435.213/0221-10', 1, 2, 1, 2, 3 , 150.00, TO_DATE('03-07-2012', 'DD-MM-YYYY'), 'alimentação');
INSERT INTO auxilio VALUES('12.435.213/0221-10', 1, 2, 1, 2, 4 , 150.00, TO_DATE('03-07-2012', 'DD-MM-YYYY'), 'alimentação');
INSERT INTO auxilio VALUES('23.543.231/0211-02', 1, 2, 1, 2, 5 , 200.00, TO_DATE('04-07-2012', 'DD-MM-YYYY'), 'transporte' );
INSERT INTO auxilio VALUES('23.543.231/0211-02', 1, 2, 1, 2, 6 , 200.00, TO_DATE('04-07-2012', 'DD-MM-YYYY'), 'transporte' );
INSERT INTO auxilio VALUES('66.232.656/0111-03', 1, 2, 1, 2, 7 , 200.00, TO_DATE('05-07-2012', 'DD-MM-YYYY'), 'transporte' );
INSERT INTO auxilio VALUES('66.232.656/0111-03', 1, 2, 1, 2, 8 , 200.00, TO_DATE('05-07-2012', 'DD-MM-YYYY'), 'transporte' );
INSERT INTO auxilio VALUES('12.878.232/0131-28', 1, 2, 1, 2, 9 , 150.00, TO_DATE('06-07-2012', 'DD-MM-YYYY'), 'alimentação');
INSERT INTO auxilio VALUES('12.878.232/0131-28', 1, 2, 1, 2, 10, 150.00, TO_DATE('06-07-2012', 'DD-MM-YYYY'), 'alimentação');
/
INSERT INTO auxilio VALUES('12.435.213/0221-10', 1, 3, 1, 3, 1 , 500.00, TO_DATE('11-06-2013', 'DD-MM-YYYY'), 'hospedagem' );
INSERT INTO auxilio VALUES('12.435.213/0221-10', 1, 3, 1, 3, 2 , 500.00, TO_DATE('11-06-2013', 'DD-MM-YYYY'), 'hospedagem' );
INSERT INTO auxilio VALUES('12.435.213/0221-10', 1, 3, 1, 3, 3 , 150.00, TO_DATE('11-06-2013', 'DD-MM-YYYY'), 'alimentação');
INSERT INTO auxilio VALUES('12.435.213/0221-10', 1, 3, 1, 3, 4 , 150.00, TO_DATE('11-06-2013', 'DD-MM-YYYY'), 'alimentação');
INSERT INTO auxilio VALUES('23.543.231/0211-02', 1, 3, 1, 3, 5 , 200.00, TO_DATE('12-06-2013', 'DD-MM-YYYY'), 'transporte' );
INSERT INTO auxilio VALUES('23.543.231/0211-02', 1, 3, 1, 3, 6 , 200.00, TO_DATE('12-06-2013', 'DD-MM-YYYY'), 'transporte' );
INSERT INTO auxilio VALUES('66.232.656/0111-03', 1, 3, 1, 3, 7 , 200.00, TO_DATE('13-06-2013', 'DD-MM-YYYY'), 'transporte' );
INSERT INTO auxilio VALUES('66.232.656/0111-03', 1, 3, 1, 3, 8 , 200.00, TO_DATE('13-06-2013', 'DD-MM-YYYY'), 'transporte' );
INSERT INTO auxilio VALUES('12.878.232/0131-28', 1, 3, 1, 3, 9 , 150.00, TO_DATE('14-06-2013', 'DD-MM-YYYY'), 'alimentação');
INSERT INTO auxilio VALUES('12.878.232/0131-28', 1, 3, 1, 3, 10, 150.00, TO_DATE('14-06-2013', 'DD-MM-YYYY'), 'alimentação');
/

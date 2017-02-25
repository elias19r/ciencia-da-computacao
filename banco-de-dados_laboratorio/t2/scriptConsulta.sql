/**
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 * 
 * SCC0241 Laboratório de Bases de Dados
 * Turma 3
 * 
 * Trabalho Prático 2 - "Consultas"
 * Script de Consulta
 */

/**
 * Consulta que envolve uma função de agregação (COUNT) incluindo as cláusulas GROUP BY e HAVING
 *   
 * 1) Selecionar o nome e a instituição das pessoas que participaram de pelo menos 3 edições de algum Simpósio
 *    e exibir o resultado em ordem alfabética dos nomes das pessoas.
 */
SELECT p.nomePe AS "Nome", 
       p.instituicaoPe AS "Instituição" 
       FROM Pessoa p JOIN Inscrito i ON  p.idPe  = i.idPart
                     JOIN Edicao  ed ON  i.codEv = ed.codEv AND i.numEd = ed.numEd
                     JOIN Evento  ev ON ed.codEv = ev.codEv
       WHERE UPPER(ev.nomeEv) LIKE UPPER('%Simpósio%')
                     GROUP BY (p.nomePe, p.instituicaoPe)
                     HAVING COUNT(p.nomePe) >= 3
       ORDER BY (p.nomePe);

/* 
 * Resultado da consulta: 
 *
Nome                                               Instituição                                      
-------------------------------------------------- --------------------------------------------------
Antonio Ribeiro                                    Universidade Estadual de Campinas                  
Bruno Burver                                       Universidade de São Paulo                          
Caio Wara                                          Universidade de São Paulo                          
Frederico Mello                                    Instituto Federal de Computação                    
Helena Delgha                                      Universidade de São Paulo                          
Hugo Silveira                                      Instituto Federal de Computação                    
João Silva                                         Instituto Federal de Computação                    
Marcelo Henrique                                   Universidade Estadual de Campinas                  
Maria selva                                        Instituto Tecnológico de Alagoas                   
Rodrigo Schutzer                                   Instituto Federal de Computação                    

 10 rows selected 
-------------------------------------------------- --------------------------------------------------
*/

/**
 * Consulta que envolve uma cláusula IN
 *
 * 2) Selecionar o nome e instituição das pessoas, além do título do artigo escrito por ela 
 *    que possua a palavra Internet em seu título.
 */
SELECT p.nomePe AS "Nome",
       p.instituicaoPe AS "Instituição",
       a.tituloArt AS "Título do Artigo"
       FROM Pessoa p JOIN Escreve esc ON p.idPe = esc.idAut
                     JOIN Artigo a ON esc.idArt = a.idArt
       WHERE a.tituloArt IN ( 
               SELECT tituloArt FROM Artigo WHERE UPPER(tituloArt) LIKE UPPER('%internet%')
       );

/*
 * Resultado da consulta : 
 * 
Nome                                               Instituição                                        Título do Artigo                                                                                   
-------------------------------------------------- -------------------------------------------------- ----------------------------------------------------------------------------------------------------
Bruno Burver                                       Universidade de São Paulo                          A Descentralização da Internet e o que Você tem a Ver com Isso                                       
Helena Delgha                                      Universidade de São Paulo                          A Descentralização da Internet e o que Você tem a Ver com Isso                                       
Hugo Silveira                                      Instituto Federal de Computação                    Marco Civil da Internet                                                                              
Rodrigo Schutzer                                   Instituto Federal de Computação                    Marco Civil da Internet                                                                              
Antonio Ribeiro                                    Universidade Estadual de Campinas                  Mudanças na nova geração de usuário de Internet                                                      
João Silva                                         Instituto Federal de Computação                    Mudanças na nova geração de usuário de Internet                                                      

 6 rows selected 
-------------------------------------------------- -------------------------------------------------- ----------------------------------------------------------------------------------------------------
*/

/**
 * Consulta que envolve uma operação de conjunto (MINUS) e é uma consulta de divisão
 *
 * 3) Liste os nomes dos organizadores dos eventos que já foram patrocinados por todos os patrocinadores.
 */
SELECT pe.nomePe AS "Organizador"
       FROM Evento ev
       JOIN Organiza org ON  ev.codEv = org.codEv 
       JOIN Pessoa pe    ON org.idOrg = pe.idPe
       WHERE NOT EXISTS
       (
            ( SELECT p.cnpjPat
              FROM patrocinador p
            )
            MINUS
            ( SELECT pa.cnpjPat
              FROM patrocinio pa
              WHERE ev.codEv = pa.codEv
            )
       )
       GROUP BY pe.nomePe;

/* 
 * Resultado da consulta: 
 *
Organizador                                      
--------------------------------------------------
Marcelo Henrique                                   
Antonio Ribeiro                                    
Bruno Burver                                       
Caio Wara                                          
Helena Delgha      
--------------------------------------------------
*/

/**
 * Consulta que envolve uma cláusula CUBE
 *
 * 4) Selecionar o valor individual das despesas das edições dos eventos, bem como a soma de cada uma delas
 *    de acordo com sua descrição. Retornar também os custos totais das despesas em todas as edições dos eventos
 *    e por cada descrição.
 */
SELECT ev.nomeEv AS "Nome do Evento",
       ed.numEd  AS "Número da Edição",
       d.descricaoDesp AS "Descrição",
       'R$ ' || to_char(sum(d.valorDesp), 'FM999G999G999D90') AS "Valor Total das Despesas"
       FROM Despesa d JOIN Edicao ed ON  d.codEv = ed.codEv AND d.numEd = ed.numEd
                      JOIN Evento ev ON ed.codEv = ev.codEv
       GROUP BY CUBE (ev.nomeEv, ed.numEd, d.descricaoDesp);

/* 
 * Resultado da consulta : 
 *
Nome do Evento                                     Número da Edição Descrição                                                                                            Valor Total das Despesas
-------------------------------------------------- ---------------- ---------------------------------------------------------------------------------------------------- ------------------------
                                                                                                                                                                         R$ 189.000,00            
                                                                    Coquetel                                                                                             R$ 18.000,00             
                                                                    Coffee Break                                                                                         R$ 21.000,00             
                                                                    Aluguel do espaço                                                                                    R$ 30.000,00             
                                                                    Divulgação online                                                                                    R$ 30.000,00             
                                                                    Sistema de Inscrição                                                                                 R$ 15.000,00             
                                                                    Material de apresentação                                                                             R$ 15.000,00             
                                                                    Conexão a Internet e energia                                                                         R$ 30.000,00             
                                                                    Manutenção preventiva de equipamentos                                                                R$ 30.000,00             
                                                                  1                                                                                                      R$ 63.000,00             
                                                                  1 Coquetel                                                                                             R$ 6.000,00              
                                                                  1 Coffee Break                                                                                         R$ 7.000,00              
                                                                  1 Aluguel do espaço                                                                                    R$ 10.000,00             
                                                                  1 Divulgação online                                                                                    R$ 10.000,00             
                                                                  1 Sistema de Inscrição                                                                                 R$ 5.000,00              
                                                                  1 Material de apresentação                                                                             R$ 5.000,00              
                                                                  1 Conexão a Internet e energia                                                                         R$ 10.000,00             
                                                                  1 Manutenção preventiva de equipamentos                                                                R$ 10.000,00             
                                                                  2                                                                                                      R$ 63.000,00             
                                                                  2 Coquetel                                                                                             R$ 6.000,00              
                                                                  2 Coffee Break                                                                                         R$ 7.000,00              
                                                                  2 Aluguel do espaço                                                                                    R$ 10.000,00             
                                                                  2 Divulgação online                                                                                    R$ 10.000,00             
                                                                  2 Sistema de Inscrição                                                                                 R$ 5.000,00              
                                                                  2 Material de apresentação                                                                             R$ 5.000,00              
                                                                  2 Conexão a Internet e energia                                                                         R$ 10.000,00             
                                                                  2 Manutenção preventiva de equipamentos                                                                R$ 10.000,00             
                                                                  3                                                                                                      R$ 63.000,00             
                                                                  3 Coquetel                                                                                             R$ 6.000,00              
                                                                  3 Coffee Break                                                                                         R$ 7.000,00              
                                                                  3 Aluguel do espaço                                                                                    R$ 10.000,00             
                                                                  3 Divulgação online                                                                                    R$ 10.000,00             
                                                                  3 Sistema de Inscrição                                                                                 R$ 5.000,00              
                                                                  3 Material de apresentação                                                                             R$ 5.000,00              
                                                                  3 Conexão a Internet e energia                                                                         R$ 10.000,00             
                                                                  3 Manutenção preventiva de equipamentos                                                                R$ 10.000,00             
Simpósio de Tecnologia da Informação (STI)                                                                                                                               R$ 189.000,00            
Simpósio de Tecnologia da Informação (STI)                          Coquetel                                                                                             R$ 18.000,00             
Simpósio de Tecnologia da Informação (STI)                          Coffee Break                                                                                         R$ 21.000,00             
Simpósio de Tecnologia da Informação (STI)                          Aluguel do espaço                                                                                    R$ 30.000,00             
Simpósio de Tecnologia da Informação (STI)                          Divulgação online                                                                                    R$ 30.000,00             
Simpósio de Tecnologia da Informação (STI)                          Sistema de Inscrição                                                                                 R$ 15.000,00             
Simpósio de Tecnologia da Informação (STI)                          Material de apresentação                                                                             R$ 15.000,00             
Simpósio de Tecnologia da Informação (STI)                          Conexão a Internet e energia                                                                         R$ 30.000,00             
Simpósio de Tecnologia da Informação (STI)                          Manutenção preventiva de equipamentos                                                                R$ 30.000,00             
Simpósio de Tecnologia da Informação (STI)                        1                                                                                                      R$ 63.000,00             
Simpósio de Tecnologia da Informação (STI)                        1 Coquetel                                                                                             R$ 6.000,00              
Simpósio de Tecnologia da Informação (STI)                        1 Coffee Break                                                                                         R$ 7.000,00              
Simpósio de Tecnologia da Informação (STI)                        1 Aluguel do espaço                                                                                    R$ 10.000,00             
Simpósio de Tecnologia da Informação (STI)                        1 Divulgação online                                                                                    R$ 10.000,00             
Simpósio de Tecnologia da Informação (STI)                        1 Sistema de Inscrição                                                                                 R$ 5.000,00              
Simpósio de Tecnologia da Informação (STI)                        1 Material de apresentação                                                                             R$ 5.000,00              
Simpósio de Tecnologia da Informação (STI)                        1 Conexão a Internet e energia                                                                         R$ 10.000,00             
Simpósio de Tecnologia da Informação (STI)                        1 Manutenção preventiva de equipamentos                                                                R$ 10.000,00             
Simpósio de Tecnologia da Informação (STI)                        2                                                                                                      R$ 63.000,00             
Simpósio de Tecnologia da Informação (STI)                        2 Coquetel                                                                                             R$ 6.000,00              
Simpósio de Tecnologia da Informação (STI)                        2 Coffee Break                                                                                         R$ 7.000,00              
Simpósio de Tecnologia da Informação (STI)                        2 Aluguel do espaço                                                                                    R$ 10.000,00             
Simpósio de Tecnologia da Informação (STI)                        2 Divulgação online                                                                                    R$ 10.000,00             
Simpósio de Tecnologia da Informação (STI)                        2 Sistema de Inscrição                                                                                 R$ 5.000,00              
Simpósio de Tecnologia da Informação (STI)                        2 Material de apresentação                                                                             R$ 5.000,00              
Simpósio de Tecnologia da Informação (STI)                        2 Conexão a Internet e energia                                                                         R$ 10.000,00             
Simpósio de Tecnologia da Informação (STI)                        2 Manutenção preventiva de equipamentos                                                                R$ 10.000,00             
Simpósio de Tecnologia da Informação (STI)                        3                                                                                                      R$ 63.000,00             
Simpósio de Tecnologia da Informação (STI)                        3 Coquetel                                                                                             R$ 6.000,00              
Simpósio de Tecnologia da Informação (STI)                        3 Coffee Break                                                                                         R$ 7.000,00              
Simpósio de Tecnologia da Informação (STI)                        3 Aluguel do espaço                                                                                    R$ 10.000,00             
Simpósio de Tecnologia da Informação (STI)                        3 Divulgação online                                                                                    R$ 10.000,00             
Simpósio de Tecnologia da Informação (STI)                        3 Sistema de Inscrição                                                                                 R$ 5.000,00              
Simpósio de Tecnologia da Informação (STI)                        3 Material de apresentação                                                                             R$ 5.000,00              
Simpósio de Tecnologia da Informação (STI)                        3 Conexão a Internet e energia                                                                         R$ 10.000,00             
Simpósio de Tecnologia da Informação (STI)                        3 Manutenção preventiva de equipamentos                                                                R$ 10.000,00             

 72 rows selected
-------------------------------------------------- ---------------- ---------------------------------------------------------------------------------------------------- ------------------------
*/

/**
 * Consulta que envolve uma cláusula EXISTS
 *
 * 5) Selecionar os eventos e suas respectivas edições em que o patrocínio dado por alguma Fundação
 *    foi usado para pagar algum auxílio.
 */
SELECT ev.nomeEv AS "Nome do Evento",
       ed.numEd AS "Edição"
       FROM Evento ev JOIN Edicao ed ON ev.codEv = ed.codEv
       WHERE EXISTS (
           SELECT p.razaoSocialPat 
               FROM Auxilio a JOIN Patrocinio pa ON a.cnpjPat = pa.cnpjPat AND a.codEvPat = pa.codEv AND a.numEdPat = pa.numEd
                              JOIN Patrocinador p ON pa.cnpjPat = p.cnpjPat
               WHERE p.razaoSocialPat LIKE '%Fundação%' AND ed.numEd = pa.numEd
       );

/* 
 * Resultado da consulta : 
 *
Nome do Evento                                         Edição
-------------------------------------------------- ----------
Simpósio de Tecnologia da Informação (STI)                  1 
Simpósio de Tecnologia da Informação (STI)                  2 
Simpósio de Tecnologia da Informação (STI)                  3 
-------------------------------------------------- ----------
*/


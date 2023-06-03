
Exemplos de programas com comunicação e sincronização entre processos
usando funções IPC Sistema V (mensagens, memória partilhada, semáforos) -- CRA
------------------------------------------------------------------------------

# Mensagens (caixas do correio):

"msg.c" -- Exemplo de utilização de mensagens IPC com 2 processos
   dependentes hierarquicamente (fork).

"msgs.c" e "msgc.c" -- Exemplo de utilização de mensagens IPC com 2
   processos independentes hierarquicamente (Cliente, Servidor).
   Executar primeiro (em "background" ou noutra janela) o programa
   Servidor ("msgs"), e depois o programa Cliente ("msgc").


# Memória partilhada:

"shm.c" -- Exemplo de utilização de memória partilhada IPC com 2 processos 
   dependentes hierarquicamente (fork).

"shmp.c" e "shmc.c" -- Exemplo de utilização de memória partilhada IPC
   com 2 processos independentes hierarquicamente (Produtor,
   Consumidor).  Executar primeiro o programa Produtor ("shmp"), e
   depois o programa Consumidor ("shmc").


# Semáforos:

"sem.c" -- Exemplo de utilização de semáforos IPC com 2 processos
   dependentes hierarquicamente (fork).

"sem1.c" e "sem2.c" -- Exemplo de utilização de semáforos IPC com 2
   processos independentes hierarquicamente.  Executar primeiro (em
   "background" ou noutra janela) o programa "sem1", e depois o
   programa "sem2".


Exemplos de programas com comunica��o e sincroniza��o entre processos
usando fun��es IPC Sistema V (mensagens, mem�ria partilhada, sem�foros) -- CRA
------------------------------------------------------------------------------

# Mensagens (caixas do correio):

"msg.c" -- Exemplo de utiliza��o de mensagens IPC com 2 processos
   dependentes hierarquicamente (fork).

"msgs.c" e "msgc.c" -- Exemplo de utiliza��o de mensagens IPC com 2
   processos independentes hierarquicamente (Cliente, Servidor).
   Executar primeiro (em "background" ou noutra janela) o programa
   Servidor ("msgs"), e depois o programa Cliente ("msgc").


# Mem�ria partilhada:

"shm.c" -- Exemplo de utiliza��o de mem�ria partilhada IPC com 2 processos 
   dependentes hierarquicamente (fork).

"shmp.c" e "shmc.c" -- Exemplo de utiliza��o de mem�ria partilhada IPC
   com 2 processos independentes hierarquicamente (Produtor,
   Consumidor).  Executar primeiro o programa Produtor ("shmp"), e
   depois o programa Consumidor ("shmc").


# Sem�foros:

"sem.c" -- Exemplo de utiliza��o de sem�foros IPC com 2 processos
   dependentes hierarquicamente (fork).

"sem1.c" e "sem2.c" -- Exemplo de utiliza��o de sem�foros IPC com 2
   processos independentes hierarquicamente.  Executar primeiro (em
   "background" ou noutra janela) o programa "sem1", e depois o
   programa "sem2".

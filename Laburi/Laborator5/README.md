# Laborator 5

## oneProducerOneConsumer:
In cadrul problemei, aveam un producator si un consumator si un buffer de
dimensiune 1, in care puteau fi introduse date, de producator, si preluate
de catre consumator. Avand dimensiunea 1, asupra bufferului se poate realiza
o singura actiune: fie de a introduce ceva (daca nu este plin), fie de a lua
ceva din el, daca nu este gol.
Bufferul este initializat cu -1, valoare default. Astfel, consideram ca bufferul
este plin, daca in interiorul sau e o valoare > 0 (producatroul introducand doar
valori > 0). Totodata, cand valoarea este -1, bufferul este gol.
In metoda put, mai intai producatorul verifica daca bufferul este gol, iar in caz
afirmativ, va introduce valoarea in buffer si apoi va notifica consumatorul. In caz
contrar, acesta va apela wait si va intra in asteptare, pana cand va fi notificat.
Abia apoi, va putea introduce o valoare in buffer.
Metoda get descrie comportamentul unui consumator. El va verifica inatai daca bufferul
nu este gol (aici bufferul e plin), si in caz afirmativ, va prelua valoarea din buffer,
inlocuind-o cu -1 pentru a demonstra faptul ca bufferul este gol, si apoi va notifica
producatorul. In caz contrar, va apela wait si va astepta sa fie notificat.
Ambele metode sunt synchronized, pentru a evita un posibil race condition in cazul scrierii
in variabila bufferului.

## multipleProducersMultipleConsumers:
 In cadrul problemei, aveam mai multi producatori si consumatori si un buffer de
 dimensiune 1, in care puteau fi introduse date, de un producator o data, si preluate
 de catre un consumator la un moment de timp.
 Metodele put si get, tot sincronizate sunt implementate asemanator exercitiului precedent,
 cu cateva mici modificari. Este folosit acum notifyAll() in loc de notify, pentru a putea
 trezi, la un moment dat, toate threadurile aflate in wait. Totodata, testarea conditiei de
 buffer plin / gol se realizeaza intr-un while, pentru a putea, in cazul trezirii a mai multor
 threaduri de tip consumator (sau producator), doar unul sa poata trece mai departe, restul fiind
 din nou puse in asteptare.

## multipleProducersMultipleConsumersNBuffer:
Problema este asemanatoare celorlalte descrise anterior. Astfel, pe langa un numar diferit de 1 de
producatori si consumatori, acum ei au la dispozitie si un buffer de dimensiune max > 1. Implementarea
celor doua metode put si get este aceeasi ca si in cadrul problemei multipleProducerMultipleConsumers,
cu mentiunea ca verificarea starii bufferului este diferita. Producatorii, la fel, pot introduce in
buffer, atata timp cat in buffer nu avem deja numarul maxim de elemente. Consumatorii pot prelua
pe rand, cate un element din buffer, doar cand bufferul nu este gol.

## philosophersProblem:
Avem un numar N de filosofi(threaduri) si un numar N de tacamuri(lock-uri).
Pentru rezolvarea problemei si evitarea unui deadlock, am implementat urmatoarea solutie: toti cei
primii N-1 filosofi vor face lock pe tacamul din stanga, si apoi pe cel din dreapta. Doar ultimul va
face lock intai pe cel din dreapta, apoi pe cel din stanga. Primul filosof, din cauza faptului ca ultimul
filosof asteapta tacamul din dreapta, va putea sa il foloseasca pe cel comun lor si sa manance, eliberand
tacamul asteptat de cel din stanga sa (adica al doilea filosof). Rand pe rand fiecare filosof va manca. In
final, si ultimul va primi lock-ul din dreapta, iar cel din stanga fiind liber, va putea manca.

## barber:
Pentru rezolvarea problemei barbierului, am utilizat mai multe semafoare binare:
clientsSem (initializa cu 0), chairsSem (cu 1) si barberReadySem (cu 0). Intai este pronit threadul barber
in executie, urmat de clienti.

Logica Barber:
Intai, el va incerca sa faca un acquire pe clientsSem. Fiind initializat cu 0, barberul va astepta pana cand primul
client va face release. Dupa ce va face acquire, barbierul va incerca sa faca acquire pe chairsSem, pentru preluarea
clientului si deci cresterea numarului de locuri disponibile in asteptare. Apoi, va face release la barberReadySem,
Semnalizand clientului ca se poate ocupa de el. Apoi se va ocupa de client.

Logica Client:
Intai, clientul va face un acquire pe chairsSem, pentru a ocupa un loc pe scaun, si sa astepte. Daca nu mai sunt
scaune libere, va face release la semafor. Daca mai sunt scaune libere, clientul va scadea numarul lor cu 1 si va face
release la clientsSem. Abia dupa primul release a lui clientsSem, barbierul va putea face acquire si sa isi inceapa
munca. Clientul va face si un chairSem release pentru a permite si altor clienti sa intre in asteptare. El va incerca
apoi sa faca aquire pe barberReady. Primul client va reusi sa faca acquire numai dupa primul release al semaforului,
realizat de barbier. Clientul va fi apoi servit.

Prin utilizarea celor doua semafoare initializate cu 0, se asteapta release-ul intai de la client, si apoi de la barbier,
astfel sincronizandu-se cele doua parti la inceput.

// Cu sleep
In cazul in care dupa pornirea unui thread client se asteapta o perioada (prin ThreadSleep) , barbierul va avea timp
sa preia clientul anterior, inainte ca urmatorul sa soseasca. Astfel, cat timp vin clienti, mereu vor fi doua scaune
din trei libere, un client fiind in asteptare si cel anterior fiind la barbier.

// Comentare sleep
In cazul in care nu exista un delay la pornirea clientilor, se vor ocupa pe rand scaunele de asteptare. Astfel, vor
fi si clienti care trebuie sa plece, fara a fii serviti.

## readersWriters:
### readerPriority: (excludere mutuala)
Pentru implementare am utiliza 2 semafoare: readWriteSem (ce permite trecerea unui cititor / scriitor in zona de
scris / citit) si numberOfReaders (ce permie intrarea intr-o zona critica, in care se modifica numarul de cititori
curenti). In urma rularii, dupa intrarea primului scriitor si apoi cititor, toti ceilalti scriitori vor astepta pana
cand toti cititorii au terminat.

Logica Reader:
Va face acquire pe semaforul numberOfReaders si va incrementa numarul de cititori. Daca este primul cititor, va face
acquire si pe readWriteSem, pentru a bloca scriitorii, si apoi va da release la semaforul initial. Dupa ce citeste, va
face acquire tot pe numberOfReaders pt a scadea numarul de cititori. Inainte de a iesi din zona critica, daca este
ultimul cititor, va da release la semaforul readWriteSem.

Logica Writer:
Scriitorul va incerca sa faca acquire pe semaforul readWriteSem. Cand trece de semafor, va scrie, iar apoi va da
release la semafor.

### writerPriority: (sincronizare conditionata)
Pentru implementare au fost utilizate 3 semafoare: enterSem(ce permite intrarea, a scriitorilor sau cititorilor) ,
readerSem (folosit pentru a introduce in asteptare cititorii) si writerSem (pentru a face scritorii sa astepte).

Logica Reader:
Cititorul incerca sa intre in zona critica, printr-un acquire pe semaforul enterSem. Daca sunt exista scriitori care
care lucreaza sau sunt in asteptare, citiroul va deveni un cititor ce asteapte, va ceda accesul in zona critica
(enterSem.release()) si va face un aquire pe readerSem. Daca nu, el va fi considerat un scriitor activ. Daca mai
sunt alti scriitori in aseptare, inseamna ca unul a iesit, deci va scadea nu si se va face release la semaforul
readerSem. Daca nu, se va face release pe zona critica.
Inainte de a citi, scriitorul tb sa solicite din nou accesul prin semaforul enterSem. In final, daca numarul de
cititori activ este 0 si mai sunt scriitori ce asteapta, se va mai face un release pe semaforul readerSem. In cazul
in care mai sunt cititori activi dar nu mai sunt cititori in asteptare, se va da release la enterSem, permitand
intrarea altor scriitori.

Logica Writer:
Scriitorul incearca sa intre in zona critica, facand acquire pe semaforul enterSem. Daca exista cititori activi sau
scriitori, atunci scriitorul va intra in aseptare (writerSem.acquire()) si va da release la enterSem.
Altfel, scriitorul va fi considerat unul activ si va da release la enterSem.
Dupa ce scrie, se va face din nou acquire (pe enterSem) pt a intra in zona critica. Scriitorul e eliminat ca fiind
unul activ. Daca mai sunt cititori in asteptare si nu mai sunt scriitori in aseptare, atunci se va elibera un
cititor, printr-un release la writerSem. Altfel, daca mai exista scriitori inactivi, se va face release la
writerSem. In cazul in care nu mai sunt nici alti cititori sau scriitori in asteptare, se va da release la enterSem,
pentru a ceda intrarea in zona critica.

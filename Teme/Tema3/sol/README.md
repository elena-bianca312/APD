Dumitru Elena-Bianca
331CC

# Tema 3 - Algoritmi Paraleli si Distribuiti
# Calcule colaborative in sisteme distribuite

Tema se bazeaza pe lucrul cu API-ul MPI, avand ca scop trimiterea de mesaje
intre diferite procese, conform unei topologii date.

Obs!
Pentru a evidentia flow-ul transmiterii de mesaje, avem functia printLog care
printeaza la stdout comunicarea dintre sender si receiver. Aceasta este apelata
de fiecare data cand apelam functia MPI_Send().

Deoarece folosim MPI, programul trebuie initializat MPI_Init() si trebuie initi-
alizat numarul de procese MPI_Comm_size(), precum si rangul lor MPI_Comm_rank().

## Exercitiul 1 - aflarea topologiei

Procesele date comunica urmand o topologie care dicteaza ce fel de comunicatii
sunt permise intre acestea. Stim de la inceput ca de fiecare data vor fi trei
clustere cu cate un proces coordonator, avand rank-urile: 0, 1, respectiv 2.

**
Astfel procesele cu aceste 3 rank-uri vor putea sa citeasca din fisier rank-uri-
le proceselor din clusterul propriu. Fiecare proces va avea 3 vectori cluster0,
cluster1 si cluster2 care vor fi populati pe parcurs.

**
Urmatorul pas este ca procesele coordonatoare sa informeze workerii din
clusterele lor cine le este coordonator, trimitandu-le rank-ul.

**
Acum informatia despre clusterele trebuie sa ajunga la toti coordonatorii.
Pentru exercitiul 1, toti coordonatorii pot comunica intre ei, asadar avem 2
for-uri intercalate pentru a realiza schimbul de informatii. Vom folosi niste
variabile auxiliare pentru a putea generaliza operatiile si a nu avea cod du-
plicat.

Pentru bonus, cand eroarea exista (=1), adica avem erori pe canalul de comuni-
catie, legatura dintre procesul 0 si procesul 1 este avariata. Deci, informa-
tiile dintre aceste 2 procese trebuie transmise prin intermediul procesului 2.
Avem o conditie suplimentara, iar in for se poate comunica doar cu procesul 2.

Ulterior, mai facem niste send-uri suplimentare prin procesul 2.

**
Impartim comunicatorul principal MPI_COMM_WORLD in subcomunicatoare pentru a
transmite mai usor informatia in cadrul unui cluster. Procesele vor fi imparti-
te in functie de rank-ul coordonatorului clusterului din care fac parte (re-
prezentand culoarea din noul subcomunicator). Noul comunicator: custom_group.

**
In momentul de fata, niciun worker nu are vreo informatie despre topologie.
Astfel, fiecare coordonator trebuie sa faca broadcast pe subcanalul sau cu in-
formatii despre toate clusterele. Acest lucru este posibil deoarece fiecare 
coordonatorul are rank-ul 0 in subcanalul nou.

Deoarece clusterele nu contin rank-ul coordonatorilor, dar sunt denumite intui-
tiv cu 0, 1 sau 2, toate procesele vor sti acum in mod corect si complet topolo-
gia.

**
Fiecare proces va printa rank-ul sau + topologia in formatul corespunzator.


## Exercitiul 2 - realizarea calculelor

**
Procesul 0, master-ul exercitiului, va citi din linia de comanda dimensiunea 
unui vector ce va trebui prelucrat si il va popula cu numere de la 0 la n - 1.

Vectorul 0 va trebui prelucrat de catre workeri. Asadar, procesul 0 va trebui sa
imparta vectorul intr-un numar echilibrat de elemente / task-uri in functie de 
numarul total de workeri, indiferent de cluster.

Se calculeaza astfel procentajele. Pentru ca tot vectorul trebuie prelucrat si 
exista posibilitatea unei impartiri cu rest, ultimul cluster se va ocupa de par-
tea nealocata, chiar daca asta s-ar putea sa insemne mai multe elemente de pre-
lucrat.

Procesul 0 trimite partile care trebuie prelucrate catre procesele 1 si 2.
Pentru bonus, procesul 0 trimite informatia despre 1 prin 2 si 2 mai departe 
catre 1.

**
Odata ce coordonatorii au primit partea din vector de care trebuie sa se ocupe,
trebuie sa o imparta in mod egal in functie de workerii lor si sa o trimita ca-
tre acestia.

Am facut un for pentru generalizare, in functie de procesul coordonator care 
trebuie sa transmita informatia. Am folosit variabile auxiliare.

**
Fiecare proces worker face prelucrarile necesare pe portiunea de vector primita
(inmultirea cu 2).

** 
Workerii trimit vectorul inapoi catre coordonatori. Deoarece coordonatorii as-
teapta mesajele de receive in ordinea rank-urilor din cluster (printr-un for),
acestia vor inregistra vectorul in ordinea corecta.

**
Coordonatorii trebuie sa trimita partea lor din vector inapoi catre procesul 0.
Daca avem eroare, informatia clusterului 1 trebuie sa treaca prin procesul 2
pentru a ajunge la procesul 0.

**
Procesul 0 printeaza rezultatul final.
























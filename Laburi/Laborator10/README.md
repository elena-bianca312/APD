# Laborator 10

## Ex1:
Am ales liderul folosind algoritmul hearbeat: la inceput fiecare nod se
considera lider, apoi am parcurs vecinii si am trimis liderul cunoscut,
am asteptat raspuns si am actualizat liderul folosind functia max.

## Ex2:
Am realizat arborele de acoperire cu ajutorul algoritmului unda-ecou:am
parcurs vecinii si am trimis propriul vector de parinti doar vecinilor care
nu sunt parintii procesului curent. Se asteapta raspuns de la fiecare proces
vecin care nu este parintele procesului si actualizam vectorul de parinti, daca
este nevoie. Orice proces ce nu este lider va trimite vectorul de vecini 
parintelui lui pentru a se actualiza topologia. Cand topologia este completa,
procesul curent va trimite topologia copiilor sai.

## Ex3:
Calculam nr. de noduri folosind algoritmul epidemic: se parcurge vectorul
de vecini, se trimite valoarea pe care o cunosc si se astepta raspuns. Valoarea
cautata este media dintre cele doua.

## Ex4:
Realizarea matricii de topologie a clusterului: se primesc informatii de la
toti copiii si se actualizeaza matricea de topologie. Se parcurg nodurile, iar 
nodul curent trimite matricea de topologie catre parinte rand cu rand. Daca 
nodul curent nu este lider, primeste topologia de la parinte. La final, se 
trimite topologia completa catre copii.

# Laborator 2

## ex 1
Am rezolvat problema de sincronizare aplicand functiile de lock si unlock
inainte si dupa adunare

## ex 2
Ca sa obtinem acelasi output mereu, am pus o bariera astfel incat thread-urile
"sa se astepte" unul pe altul

## ex 3 & 4
Am calculat pozitiile de start si end si am paralelizat programul prin 
împărțirea iterației la mai multe thread-uri

## ex 5
Am calculat pozitiile de start si end si am paralelizat bucla interioara.
Am folosit o variabila auxiliara "var" pentru inmultire si am facut adunarea
sumelor partiale in c[][].

## Bonus
Folosind ideea lui strasse, am realizat calculele pentru matricea finala
olosind 7 operații in loc de 8 pentru cazul clasic (for-uri imbricate sau 
recursiv prin divide et impera). Astfel, am lansat 7 thread-uri de execuție pt
a genera cele 7 matrice. După finalizarea acestei etape, am realizat cele 4 
calcule finale pt compunerea matricei C.

Dumitru Elena-Bianca
331CC

# Tema 2 - Algoritmi Paraleli si Distribuiti
# Procesarea de documente folosind paradigma Map-Reduce

In aceasta tema, am implementat un program paralel de parsare, prelucrare si in-
terpretare a datelor din mai multe fisiere.

Logica programului este implementata de clasa Parser.

## Parser

In Parser, lansam un ExecutorService care va prelua toate tipurile de task-uri
generate pe parcurs si le va asigna workerilor in functie de numarul de workeri
cu care este asignat (citit de la tastatura).

### constructor

In constructor, primim fisierele de intrare si iesire si populam in continuare 
campurile relevante: numarul de fisiere, numele lor.

### makeTasks

In functia makeTasks, generam task-urile de tip map si reduce si le retinem re-
zultatele in liste corespunzatoare. Ambele task-uri implementeaza interfata
Runnable pentru a putea intoarce obiecte cu informatiile necesare.

Dupa realizarea ambelor tipuri de task-uri, inchidem executorService-ul pentru
a nu prelungi durata programului, iar apoi printam rezultatul in fisierul de
output.

### map

Genereaza task-uri de tip mapReduce, ExecutorService le executa, iar rezultatul
va fi intors printr-o lista de MapResult.

### reduce ~ analog map

### executeAndCollect

Functie generica care invoca workerii din ExecutorService si strange rezultatele

### computeOutput

Functie care sorteaza rezultatele finale in functie de rang si scrie rezultatul
la output in formatul corespunzator.


## Map Function

Computes dictionary and list with words with maximum length.

## Reduce Function

Stores results of Map Function tasks

## Reduce Function

Merges dictionaries and lists from all files. Computes file rank, maximum word
length and how many words have the maximum length. 

## Reduce Function

Stores results of Map Function tasks. Formats output.



Link-uri utile:
https://howtodoinjava.com/java/multi-threading/executorservice-invokeall/























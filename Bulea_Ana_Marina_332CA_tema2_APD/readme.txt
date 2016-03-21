Tema 2 APD 2015 - Bluea Ana-Marina 332CA


Am implementat tema plecand de la templateul laboratorului 5.
Am mutat functia de procesare a workerului in clasa PartialSolution
si din ea am derivat clasele MapPartialSolution care se ocupa cu procesarea
facuta in operatia de map si ReducePartialSolution care se ocupa cu
etapele de combinare si procesare finale. Clasa Solution descrie rezultatul
operatiilor de map si reduce (rankul documentului, cuvintele maximale,
numarul de aparitii).

In MapPartialSolution se citeste intr-un buffer fragmentul din fisier
si se verifica daca incepe sau se termina in mijlocul unui cuvant. Se
construieste arrayul "fragment" ce contine fragmentul efectiv si se parcurge
pentru a se determina si procesa cuvintele. In ReducePartialSolution se
combina toate rezultatele de la MapPartialSolution si se calculeaza rankul.

MapPartialSolution primeste ca parametrii numele documentrului, pozitia
de inceput a fragmentului, lungimea fragmentului si lista in care se va
stoca rezultatul. Fiecare document are o lista corespunzatoare pentru
rezultatele mapului. La operatia de reduce se da ca parametru lista
corespunzatoare unui document, ReducePartialSolution proceseaza informatia
finala pe care o pune in lista documentului dupa ce aceasta este golita.
In final se obtine un ArrayList<LinkedList<Solution>> care contine pentru
fiecare document o lista cu metricile documentului.

Pentru sortarea documentelor la output am declarat clasa MyComparator
si am folosit functia Collections.sort.
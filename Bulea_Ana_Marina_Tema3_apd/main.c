//Tema 3 APD 2015 - Bluea Ana-Marina 332CA

#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>

#define PARENT_TAG 1
#define PARENT_ACCEPT_TAG 2
#define PARENT_REJECT_TAG 3

typedef struct{
	//matrice de adiacenta
	char matrix[16][16];
	//tabela de rutare
	char table[16];
} NodeTopology;

typedef struct{
	char matrix[16][16];
} Sudoku;

//citeste lista de vecini si ii marcheaza in vectorul neighbours
void read_neighbours(char *line, int rank, int *neighbours){
	char *p;
	int i;

	p = strtok(line, " ");

	while(p != NULL){
		i = atoi(p);
		neighbours[i] = 1;

		p = strtok(NULL, " ");
	}
}

//ruleaza algoritmul de stabilire a topologiei
void build_topology(int rank, int numtasks, int *neighbours, NodeTopology *topology, int *parent){
	int i, j, n, flag, temp;
	NodeTopology child_topology; //topologia primita de la un copil
	MPI_Status status;

	if(rank != 0){
		//primeste prima cerere de parinte
		MPI_Recv(parent, 1, MPI_INT, MPI_ANY_SOURCE, PARENT_TAG, MPI_COMM_WORLD, NULL);
	}
	else *parent = -1;

	//umple tabela de rutare cu id-ul parintelui
	for(i = 0; i < numtasks; i++)
		topology->table[i] = *parent;
	topology->table[rank] = -1;

	//trimite cereri de parinte vecinilor
	n = 0;
	for(i = 0; i < numtasks; i++){
		if(neighbours[i] && i != *parent){
			MPI_Send(&rank, 1, MPI_INT, i, PARENT_TAG, MPI_COMM_WORLD);
			n++;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	//rejecteaza toate celelalte cereri de parinte primite
	while(1){
		//verifica daca mai sunt mesaje de tipul PARENT_TAG in coada
		MPI_Iprobe(MPI_ANY_SOURCE, PARENT_TAG, MPI_COMM_WORLD, &flag, NULL);
		if(!flag){
			break;
		}

		MPI_Recv(&temp, 1, MPI_INT, MPI_ANY_SOURCE, PARENT_TAG, MPI_COMM_WORLD, &status);
		MPI_Send(&rank, 1, MPI_INT, status.MPI_SOURCE, PARENT_REJECT_TAG, MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	//numara copii care au rejectat cerereea de parinte
	while(n > 0){
		MPI_Iprobe(MPI_ANY_SOURCE, PARENT_REJECT_TAG, MPI_COMM_WORLD, &flag, NULL);
		if(!flag){
			break;
		}

		//scoate mesajul din coada
		MPI_Recv(&temp, 1, MPI_INT, MPI_ANY_SOURCE, PARENT_REJECT_TAG, MPI_COMM_WORLD, &status);
		n--;
	}

	//primeste topologia de la copii care au acceptat cererea de parinte
	while(n > 0){
		MPI_Recv(&child_topology, sizeof(child_topology), MPI_CHAR, MPI_ANY_SOURCE,
		         PARENT_ACCEPT_TAG, MPI_COMM_WORLD, &status);

		topology->matrix[rank][status.MPI_SOURCE] = 1;

		//completeaza matricea de adiacenta a nodului cu cea primita de la copil
		for(i = 0; i < numtasks; i++){
			for(j = 0; j < numtasks; j++){
				topology->matrix[i][j] = topology->matrix[i][j] || child_topology.matrix[i][j];
			}
		}

		//completeaza tabela de rutare cu cea primita de la copil
		topology->table[status.MPI_SOURCE] = status.MPI_SOURCE;
		for(i = 0; i < numtasks; i++){
			if(i != rank && child_topology.table[i] != -1 && child_topology.table[i] != rank)
				topology->table[i] = status.MPI_SOURCE;
		}

		n--;
	}

	//trimite topologia sa la parinte
	if(*parent != -1){
		MPI_Send(topology, sizeof(*topology), MPI_CHAR, *parent, PARENT_ACCEPT_TAG, MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);
}

//verifica sudoku-ul
int check_matrix(Sudoku *sudoku, int size){
	int got[17]; //numerele gasite pe linie/coloana/patrat
	int i, j, k, l, v;

	//verifica liniile
	for(i = 0; i < size * size; i++){
		memset(got, 0, sizeof(got));

		for(j = 0; j < size * size; j++){
			v = sudoku->matrix[i][j];

			if(v != 0){
				if(got[v]) //daca deja are v pe aceasta linie
					return 0;
				else got[v] = 1;
			}
		}
	}

	//verifica coloane
	for(i = 0; i < size * size; i++){
		memset(got, 0, sizeof(got));

		for(j = 0; j < size * size; j++){
			v = sudoku->matrix[j][i];

			if(v != 0){
				if(got[v]) //daca are deja v pe aceasta coloana
					return 0;
				else got[v] = 1;
			}
		}
	}

	//verifica patrate
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			memset(got, 0, sizeof(got));

			//parcurge patratul
			for(k = 0; k < size; k++){
				for(l = 0; l < size; l++){
					v = sudoku->matrix[size*i+k][size*j+l];

					if(v != 0){
						if(got[v]) //daca are deja v in acest patrat
							return 0;
						else got[v] = 1;
					}
				}
			}
		}
	}

	return 1;
}

//functie recursiva care genereaza soltuiile partiale ale sudoku-ului si le
//adauga in vectorul results
//p, q = prima patratica din patrat
//k, l = coordonata patraticii in patrat
//is_free[i] = 1 daca nu s-a folosit i in patrat
Sudoku* generate_func(Sudoku *sudoku, int size, int p, int q, int k, int l,
                      int *is_free, Sudoku *results, int *results_size){
	int next_k, next_l;

	if(l == size - 1){
		next_k = k+1;
		next_l = 0;
	}
	else{
		next_k = k;
		next_l = l+1;
	}

	if(k >= size){
		//s-a completat toata patratica, verifica sudoku-ul
		if(check_matrix(sudoku, size)){
			//adauga solutia partiala la vectorul results
			(*results_size)++;
			results = realloc(results, sizeof(Sudoku) * (*results_size));
			results[*results_size - 1] = *sudoku;
		}

		return results;
	}
	else if(sudoku->matrix[p+k][q+l] != 0){
		//patratica curenta este deja completata, trece la urmatoarea
		return generate_func(sudoku, size, p, q, next_k, next_l, is_free, results, results_size);
	}
	else{
		int i;
		int my_is_free[17];
		Sudoku solution = *sudoku;

		memcpy(my_is_free, is_free, (size * size +1)*sizeof(int));

		//incearca toate numerele nefolosite pe patratica curenta
		for(i = 1; i <= size * size; i++){
			if(is_free[i]){
				solution.matrix[p+k][q+l] = i;
				my_is_free[i] = 0;
				results = generate_func(&solution, size, p, q, next_k, next_l, my_is_free, results, results_size);
				my_is_free[i] = 1;
			}
		}
	}

	return results;
}

//genereaza toate solutiile partiale pentru patratica curenta
Sudoku* generate(int rank, Sudoku *original, int size, int *count){
	//calculeaza prima pozitie din patrat
	int p = size * (rank / size), q = size * (rank % size);
	int i, j;
	int is_free[17];

	for(i = 1; i <= size * size; i++)
		is_free[i] = 1;

	is_free[0] = 0;

	//marcheaza numerele nefolosite in is_free
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			if(original->matrix[p+i][q+j] != 0)
				is_free[original->matrix[p+i][q+j]] = 0;
		}
	}

	*count = 0;
	//apeleaza functia de generare
	return generate_func(original, size, p, q, 0, 0, is_free, NULL, count);
}

//combina sudoku-ul a cu sudoku-ul b
void combine(Sudoku *a, Sudoku *b, int size){
	int i, j;

	for(i = 0; i < size * size; i++){
		for(j = 0; j < size * size; j++)
			if(a->matrix[i][j] == 0)
				a->matrix[i][j] = b->matrix[i][j];
	}
}

//genereaza solutiile finale combinand solutiile partiale
//partial_solutions = vector de vectori de solutii partiale
//count = vector de lungimi pentru vectorii de soluti partiale
//num_solutions = numarul de solutii partiale
Sudoku *generate2(Sudoku **partial_solutions, int *count, int num_solutions, int size,
                  Sudoku *sudoku, int k, Sudoku *solutions, int *result_count){
	if(k >= num_solutions){
		//adauga solutia la vectorul de solutii
		(*result_count)++;
		solutions = realloc(solutions, sizeof(Sudoku) * (*result_count));
		solutions[*result_count - 1] = *sudoku;
	}
	else{
		int i;
		Sudoku solution;

		//combina sudoku-ul primit cu fiecare solutie partiala de la pasul k
		//verifica, si il trimite la pasul k+1 daca este valid
		for(i = 0; i < count[k]; i++){
			solution = *sudoku;
			combine(&solution, &(partial_solutions[k][i]), size);
			if(check_matrix(&solution, size))
				solutions = generate2(partial_solutions, count, num_solutions, size,
				                      &solution, k+1, solutions, result_count);
		}
	}

	return solutions;
}

//functie de afisare
void print_sudoku(FILE *file, Sudoku *sudoku, int size){
	int i, j;

	fprintf(file, "%d\n", size);
	for(i = 0; i < size * size; i++){
		for(j = 0; j < size * size; j++){
			fprintf(file, "%d ", sudoku->matrix[i][j]);
		}
		fprintf(file, "\n");
	}
}

int main(int argc, char ** argv){
	int numtasks, rank, parent;

	//dimensiunea sudoku
	int size, i, j, temp;
	//sudoku din fisier
	Sudoku sudoku;

	//lista de adiacenta pentru vecini
	int neighbours[16];

	//topologia ramurii si tabela de rutare
	NodeTopology top;

	//fisiere
	FILE *sudoku_in, *sudoku_out, *topology;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	sudoku_in  = fopen(argv[2], "r");
	topology   = fopen(argv[1], "r");
	sudoku_out = fopen(argv[3], "w");

	//initializare
	for(i = 0; i < numtasks; i++){
		for(j = 0; j < numtasks; j++){
			top.matrix[i][j] = 0;
		}

		top.table[i] = -1;
		neighbours[i] = 0;
	}

	//citeste dimensiunea sudoku-ului
	fscanf(sudoku_in, "%d", &size);

	//citeste sudoku-ul din fisier
	for(i = 0; i < size * size; i++){
		for(j = 0; j < size * size; j++){
			fscanf(sudoku_in, "%d", &temp);
			sudoku.matrix[i][j] = temp;
		}
	}

	//citeste lista de vecini
	for(i = 0; i < numtasks; i++){
		int line_rank;
		char line[256];

		fscanf(topology, "%d - ", &line_rank);
		fgets(line, 256, topology);

		if(line_rank == rank){
			read_neighbours(line, rank, neighbours);
			break;
		}
	}

	//ruleaza algoritmul de stabilire a topologiei
	build_topology(rank, numtasks, neighbours, &top, &parent);

	//afiseaza tabela de rutare pentru fiecare nod
	for(i = 0; i < numtasks; i++){
		MPI_Barrier(MPI_COMM_WORLD);
		if(rank == i){
			if(rank == 0){
				printf("Tabele de rutare:\n");
			}

			printf("%d: ", rank);
			for(j = 0; j < numtasks; j++)
				printf("(%d, %d) ", j, top.table[j]);
			printf("\n");
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	//radacina afiseaza matricea de adiacenta a retelei
	if(rank == 0){
		printf("\nMatrice de adiacenta:\n");
		for(i = 0; i < numtasks; i++){
			for(j = 0; j < numtasks; j++){
				printf("%d ", top.matrix[i][j]);
			}
			printf("\n");
		}

		printf("\n");
	}

	MPI_Barrier(MPI_COMM_WORLD);

	//solutii partiale de la copii
	//count[k] = numarul de solutii partiale primite de la al k-le copil
	//partial_solutions[i] = vectorul de solutii partiale primit de la al k-lea copil
	int count[16];
	Sudoku *partial_solutions[16];
	int num_solutions = 0;

	//genereaza solutiile partiale ale nodului curent
	count[0] = 0;
	partial_solutions[0] = generate(rank, &sudoku, size, &(count[0]));
	num_solutions++;

	//primeste solutiile partiale de la toti copii
	for(i = 0; i < size * size; i++){
		if(top.matrix[rank][i]){
			//primeste numarul de solutii
			MPI_Recv(&(count[num_solutions]), 1, MPI_INT, i, 0, MPI_COMM_WORLD, NULL);

			//aloca vector
			partial_solutions[num_solutions] = malloc(sizeof(Sudoku) * count[num_solutions]);

			//primeste vector
			MPI_Recv(partial_solutions[num_solutions],
			         sizeof(Sudoku) * count[num_solutions], MPI_CHAR, i, 0,
			         MPI_COMM_WORLD, NULL);

			num_solutions++;
		}
	}

	//genereaza solutiile finale
	int num_fsolutions = 0;
	Sudoku *solutions = generate2(partial_solutions, count, num_solutions, size,
	                              &sudoku, 0, NULL, &num_fsolutions);

	//trimite solutiile finale la parinte
	if(rank != 0){
		MPI_Send(&num_fsolutions, 1, MPI_INT, parent, 0, MPI_COMM_WORLD);
		MPI_Send(solutions, sizeof(Sudoku) * num_fsolutions, MPI_CHAR, parent, 0, MPI_COMM_WORLD);
	}
	else{
		//nodul radacia a primit toate solutiile, printeaza in fisier
		print_sudoku(sudoku_out, solutions + 0, size);
	}

	//elibereaza memorie
	for(i = 0; i < num_solutions; i++)
		free(partial_solutions[i]);

	free(solutions);

	fclose(topology);
	fclose(sudoku_in);
	fclose(sudoku_out);

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Finalize();

	return 0;
}

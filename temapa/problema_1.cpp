#include <iostream>  //cout
#include <fstream>   //ifstream/ofstream
#include <vector>    //vector
#include <algorithm> //max si min

using namespace std;

//structura unui obiect
struct object {
	int weight, value_r1, value_r2;
};

int main() {
	ifstream file; //fisier intrare
	ofstream out; //fisier iesire
	int ****vec; //matrice valoare maxima rucsac 2
	object *objects; //vector obiecte
	int n, g1, g2;
	int tmp_val, tmp_weight;
	int new_val1, new_val2, max_prev;
	int max_val1 = 0; //valoarea maxima ce se poate pune in rucsacul 1
	int max_prod, prod_found = 0;
	object obj;

	file.open("excursie.in");
	if (!file.is_open()) {
		cout << "Nu s-a putut deschide fisierul excursie.in" << endl;
		return 0;
	}

	file >> n >> g1 >> g2;
	objects=new object[n]; 
	for (int i = 0; i < n; i++) {
		file >> objects[i].weight >> objects[i].value_r1 >> objects[i].value_r2;

		max_val1 = max_val1 + objects[i].value_r1;
	}

	max_val1 = min(150, max_val1);

	file.close();

	//alocare matrice 4d
	vec = new int***[n + 1]; //obiect
	for (int i = 0; i < n + 1; i++) {
		vec[i] = new int**[g1 + 1]; //greutate rucsac1

		for (int j = 0; j < g1 + 1; j++) {
			vec[i][j] = new int*[g2 + 1]; //greutate rucsac2

			for (int h = 0; h < g2 + 1; h++) {
				vec[i][j][h] = new int[max_val1 + 1]; //valoare rucsac1

				for (int k = 0; k < max_val1 + 1; k++) {
					if (i == 0 && j == 0 && h == 0 && k == 0) {
						vec[i][j][h][k] = 0;
					} else {
						vec[i][j][h][k] = -1; //initializare cu -1
					}
				}
			}
		}
	}

	for (int i = 1; i < n + 1; i++) {
		for (int j = 0; j < g1 + 1; j++) {
			for (int h = 0; h < g2 + 1; h++) {
				for (int k = 0; k < max_val1 + 1; k++) {

					new_val1 = new_val2 = -1;
					obj = objects[i - 1];

					//incearca sa puna obiectul in rucsacul 1
					if (obj.weight <= j && obj.value_r1 <= k) {
						tmp_val = k - obj.value_r1;
						tmp_weight = j - obj.weight;

						if (vec[i - 1][tmp_weight][h][tmp_val] >= 0) {
							//valoare rucsac 2
							new_val1 = vec[i - 1][tmp_weight][h][tmp_val];
						}
					}

					//incearca sa puna obiectul in rucsacul 2
					if (obj.weight <= h) {
						tmp_weight = h - obj.weight;

						if (vec[i - 1][j][tmp_weight][k] >= 0) {
							//valoare noua rucsac 2
							new_val2 = vec[i - 1][j][tmp_weight][k]
									+ obj.value_r2;
						}
					}

					//valoarea maxima la pasul anteriori
					max_prev = vec[i - 1][j][h][k];

					//valoarea finala
					vec[i][j][h][k] = max(max_prev, max(new_val1, new_val2));
					
					//retine suma maxima
					if (prod_found == 0) {
						max_prod = vec[i][j][h][k] * k;
						prod_found = 1;
					} else if (vec[i][j][h][k] * k > max_prod) {
						max_prod = vec[i][j][h][k] * k;
					}
				}
			}
		}
	}

	out.open("excursie.out");
	if (!out.is_open()) {
		cout << "Nu s-a putut creea fisierul excursie.out" << endl;
		return 0;
	}

	out << max_prod << endl;

	out.close();

	//dezalocare matrice
	for (int i = 0; i < n + 1; i++) {
		for (int j = 0; j < g1 + 1; j++) {
			for (int h = 0; h < g2 + 1; h++) {
				delete[] vec[i][j][h];
			}

			delete[] vec[i][j];
		}

		delete[] vec[i];
	}

	delete[] vec;
	delete[] objects;

}

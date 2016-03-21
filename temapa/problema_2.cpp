#include <algorithm> //sort
#include <iostream>  //cout
#include <fstream>   //ifstream/ofstream
#include <vector>    //vector
#include <utility>   //pair

using namespace std;

//calculeaza rankul minim si maxim al unei sume
//pair.first = capatul stang al intervalului
//pair.second = capatul drept al intervalului
pair<unsigned, unsigned> rank_elem(vector<int> &v, unsigned sum) {
	int i = 0, j = v.size() - 1, cnt = 0;
	unsigned s;
	pair<unsigned, unsigned> rank;

	while (i < v.size() && j >= 0) {
		s = v[i] + v[j];

		if (s == sum) {
			rank.second++; //rank maxim
			j--;
		} else if (s < sum) {
			rank.first = rank.first + j + 1; //rank minim
			rank.second = rank.second + j + 1;
			i++;
		} else {
			j--;
		}
	}

	return rank;
}

//cautare a k-a suma
int search(vector<int> &v, unsigned k) {
	int a = v[0] * 2, b = v[v.size() - 1] * 2, mid;

	if (k == 1) {
		return a;
	} else if (k == v.size() * v.size()) {
		return b;
	}

	//cautare binara
	while (1) {
		mid = (a + b) / 2;
		pair<unsigned, unsigned> rank = rank_elem(v, mid);

		if (rank.first < k && rank.second >= k - 1) { // k in [rank.min, rank.max] 
			return mid;
		} else if (rank.second >= k - 1) { // k < rank.max
			b = mid;
		} else if (rank.first < k - 1) { // k > rank.min
			a = mid;
		} else {
			std::cout << "error";
			return -1;
		}

	}
}

int main() {
	ifstream file; //fisier intrare
	ofstream out; //fisier iesire
	unsigned n, k, kthsum;
	vector<int> v; //vector numere

	file.open("patrat.in");
	if (!file.is_open()) {
		cout << "Nu s-a putut deschide fisierul patrat.in" << endl;
		return 0;
	}

	file >> n >> k;

	for (int i = 0; i < n; i++) {
		int val;
		file >> val;
		v.push_back(val);
	}

	file.close();

	//sortatre crescatoare
	sort(v.begin(), v.end());

	kthsum = search(v, k);

	out.open("patrat.out");
	if (!out.is_open()) {
		cout << "Nu s-a putut creea fisierul patrat.out" << endl;
		return 0;
	}

	out << kthsum << endl;

	out.close();
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
using namespace std;

// Função para encontrar o representante do conjunto (com compressão de caminho)
int achar_pai(vector<int>& pai, int x) {
    if (pai[x] != x) {
        pai[x] = achar_pai(pai, pai[x]); // Compressão de caminho
    }
    return pai[x];
}

// Função para encontrar o representante do conjunto
int findSet(vector<int>& pai, int x) {
    if (pai[x] != x) {
        pai[x] = findSet(pai, pai[x]); // Compressão de caminho
    }
    return pai[x];
}

// Função para unir dois conjuntos (união por rank)
void unionSets(vector<int>& pai, vector<int>& rank, int x, int y) {
    int raizX = achar_pai(pai, x);
    int raizY = achar_pai(pai, y);

    if (raizX != raizY) {
        if (rank[raizX] > rank[raizY]) {
            pai[raizY] = raizX;
        } else if (rank[raizX] < rank[raizY]) {
            pai[raizX] = raizY;
        } else {
            pai[raizY] = raizX;
            rank[raizX]++;
        }
    }
}

// Algoritmos de ordenação
void bubble_Sort(vector<int>& vec) {
    for (size_t i = 0; i < vec.size() - 1; i++) {
        for (size_t j = 0; j < vec.size() - i - 1; j++) {
            if (vec[j] > vec[j + 1]) {
                swap(vec[j], vec[j + 1]);
            }
        }
    }
}

void insertion_Sort(vector<int>& vec) {
    for (size_t i = 1; i < vec.size(); i++) {
        int key = vec[i];
        int j = i - 1;
        while (j >= 0 && vec[j] > key) {
            vec[j + 1] = vec[j];
            j--;
        }
        vec[j + 1] = key;
    }
}

void merge(vector<int>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = vec[left + i];
    for (int j = 0; j < n2; j++) R[j] = vec[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            vec[k] = L[i];
            i++;
        } else {
            vec[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        vec[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        vec[k] = R[j];
        j++;
        k++;
    }
}

void merge_Sort(vector<int>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_Sort(vec, left, mid);
        merge_Sort(vec, mid + 1, right);
        merge(vec, left, mid, right);
    }
}

int partition(vector<int>& vec, int low, int high) {
    int pivot = vec[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (vec[j] <= pivot) {
            i++;
            swap(vec[i], vec[j]);
        }
    }
    swap(vec[i + 1], vec[high]);
    return i + 1;
}

void quick_Sort(vector<int>& vec, int low, int high) {
    if (low < high) {
        int pi = partition(vec, low, high);
        quick_Sort(vec, low, pi - 1);
        quick_Sort(vec, pi + 1, high);
    }
}

void Printar_Ordenacao(const vector<int>& vec, ofstream& arquivo) {
    for (int el : vec) {
        arquivo << el << " ";
    }
    arquivo << endl;
}

int main() {
    srand(time(0));

    ifstream file("entrada1.txt");
    if (!file) {
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        return 1;
    }

    int n, k;
    file >> n >> k;

    if (k < 1 || n < k * 2) {
        cerr << "Valores inválidos: certifique-se de que k > 1 e n >= 2k." << endl;
        return 1;
    }

    vector<int> pai(n + 1);
    vector<int> rank(n + 1, 0);
    iota(pai.begin(), pai.end(), 0); // Inicializa o vetor pai com [0, 1, 2, ..., n]

    vector<vector<int>> subconjuntos(k);
    vector<int> elementos(n);
    iota(elementos.begin(), elementos.end(), 1);
    random_shuffle(elementos.begin(), elementos.end());

    // Divisão dos elementos em subconjuntos
    int base_size = n / k; // Tamanho base
    int extra = n % k;     // Elementos restantes
    int idx = 0;

    for (int i = 0; i < k; i++) {
        int size = base_size + (i < extra ? 1 : 0);
        subconjuntos[i].insert(subconjuntos[i].end(), elementos.begin() + idx, elementos.begin() + idx + size);
        idx += size;
    }

    ofstream arquivo("resultado.txt");
    for (int i = 0; i < k; i++) {
        arquivo << "Subconjunto antes da ordenação: ";
        Printar_Ordenacao(subconjuntos[i], arquivo);

        string algoritmo;

        int sorteio = rand() % 4;
        switch (sorteio) {
            case 0: bubble_Sort(subconjuntos[i]);algoritmo = "bubble_Sort"; break;
            case 1: insertion_Sort(subconjuntos[i]);algoritmo = "insertion_Sort"; break;
            case 2: merge_Sort(subconjuntos[i], 0, subconjuntos[i].size() - 1);algoritmo = "merge_Sort"; break;
            case 3: quick_Sort(subconjuntos[i], 0, subconjuntos[i].size() - 1);algoritmo = "quick_Sort"; break;
        }

        arquivo << "Subconjunto após a ordenação ("<< algoritmo <<"): ";
        Printar_Ordenacao(subconjuntos[i], arquivo);

        for (size_t j = 1; j < subconjuntos[i].size(); j++) {
            unionSets(pai, rank, subconjuntos[i][0], subconjuntos[i][j]);
        }

        // Imprime o representante do subconjunto
        int representante = achar_pai(pai, subconjuntos[i][0]);
        arquivo << "Representante do subconjunto: " << representante << endl;
        arquivo << endl;
    }

    arquivo << "Vetor pai final: ";
    Printar_Ordenacao(pai, arquivo);

    cout << "Processo concluído, resultado salvo em 'resultado.txt'." << endl;

    return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
using namespace std;

// Funcao para encontrar o representante do conjunto (com compressao de caminho)
int achar_pai(vector<int>& pai, int x) {
    if (pai[x] != x) {
        pai[x] = achar_pai(pai, pai[x]); // Compressao de caminho
    }
    return pai[x];
}

// Funcao para encontrar o representante de um conjunto, sem modificar o conjunto
int achar_representante(const vector<int>& pai, int x) {
    while (pai[x] != x) {
        x = pai[x];  // Subir na arvore ate encontrar o representante
    }
    return x;  // Retorna o representante (raiz)
}

// Função para fazer a união de dois subconjuntos
void unionSets(vector<int>& pai, vector<int>& rank, int x, int y) {
    int raizX = achar_pai(pai, x);
    int raizY = achar_pai(pai, y);

    if (raizX != raizY) {
        // União por rank
        if (rank[raizX] > rank[raizY]) {
            pai[raizY] = raizX; // A raiz de Y agora e X
        } else if (rank[raizX] < rank[raizY]) {
            pai[raizX] = raizY; // A raiz de X agora e Y
        } else {
            pai[raizY] = raizX; // A raiz de Y agora e X
            rank[raizX]++; // Incrementa o rank de X
        }
    }
}

// Algoritmos de ordenacao
// Funcao para realizar ordenacao por meio de bubble_Sort
void bubble_Sort(vector<int>& vec) {
    for (size_t i = 0; i < vec.size() - 1; i++) {
        for (size_t j = 0; j < vec.size() - i - 1; j++) {
            if (vec[j] > vec[j + 1]) {
                swap(vec[j], vec[j + 1]);
            }
        }
    }
}

// Funcao para realizar ordenacao por meio de insertion_Sort 
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

// Funcao merge para ser usado no algoritmo de merge_Sort 
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

// Funcao para realizar ordenacao por meio de merge_Sort 
void merge_Sort(vector<int>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_Sort(vec, left, mid);
        merge_Sort(vec, mid + 1, right);
        merge(vec, left, mid, right);
    }
}

// Funcao partition usado no algoritmo de quick_Sort para dar swap nos elementos 
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

// Funcao para realizar ordenacao por meio de quick_Sort
void quick_Sort(vector<int>& vec, int low, int high) {
    if (low < high) {
        int pi = partition(vec, low, high);
        quick_Sort(vec, low, pi - 1);
        quick_Sort(vec, pi + 1, high);
    }
}

// Funcao para imprimir a ordenacao feita(não utilizado)
void Printar_Ordenacao(const vector<int>& vec, ofstream& arquivo) {
    for (int el : vec) {
        arquivo << el << " ";
    }
}

// Funcao para imprimir o representante de um subconjunto (antes da ordenacao)(não utilizado)
void imprimirRepresentante(const vector<int>& pai, const vector<int>& subconjunto, ofstream& arquivo) {
    // Encontrar o representante do subconjunto
    int representante = achar_representante(pai, subconjunto[0]);  // Encontrar o representante do primeiro elemento

    // Imprimir o representante do subconjunto
    arquivo << "Representante do conjunto: " << representante << endl;
}

// Funcao para imprimir o vetor pai
void imprimirVetorPai(const vector<int>& pai, ofstream& arquivo) {
    arquivo << "Vetor pai: " << '\n';
    for (size_t i = 1; i < pai.size(); i++) {
        if( i % 20 == 1 && i > 20){
            arquivo << '\n';
        }
        arquivo << pai[i] << " ";  // Imprime o valor do elemento no índice i
    }
    arquivo << endl;
}

// Funcao para imprimir o vetor
void imprimirVetor(const vector<int>& vec) {
    for (int el : vec) {
        cout << el << " ";
    }
    cout << endl;
}

int main() {
    srand(time(0)); // Inicializar a semente para números aleatórios(não utilizado)

    ifstream file("entrada.txt");
    if (!file) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return 1;
    }

    int n;
    file >> n;

    vector<int> pai(n + 1);
    vector<int> rank(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        pai[i] = i;
    }

    string line;
    getline(file, line); // Consumir a linha restante

    while (getline(file, line)) {
        stringstream ss(line);
        int element1, element2;
        ss >> element1;  // Pega o primeiro elemento para unir com os outros
        while (ss >> element2) {
            unionSets(pai, rank, element1, element2); // Faz a união entre os elementos
        }
    }

    file.close();

    while (true) {
        cout << "\nConjunto contem elementos de 1 a " << n;
        cout << "\nMenu de Operacoes:\n";
        cout << "1. Ordenar subconjunto contendo um elemento escolhido\n";
        cout << "2. Identificar o representante de um elemento\n";
        cout << "3. Fazer uniao de dois subconjuntos\n";
        cout << "4. Sair\n";
        cout << "Escolha uma opcao: ";
        int opcao;
        cin >> opcao;

        cout << '\n';

        if (opcao == 1) {
            int elemento;
            cout << "Digite o elemento para ordenar seu subconjunto: ";
            cin >> elemento;

            if (elemento < 1 || elemento > n) {
                cout << "Elemento invalido.\n";
                continue;
            }

            // Encontrar o representante do elemento
            int representante = achar_pai(pai, elemento);

            // Criar o subconjunto associado ao representante
            vector<int> subconjunto;
            for (int i = 1; i <= n; i++) {
                if (achar_pai(pai, i) == representante) {
                    subconjunto.push_back(i);
                }
            }

            // Ordenar o subconjunto escolhido
            cout << "Escolha o metodo de ordenacao:\n1. Bubble Sort\n2. Insertion Sort\n3. Quick Sort\n4. Merge Sort\n";
            int metodo;
            cin >> metodo;

            if (metodo == 1) {
                bubble_Sort(subconjunto);
            } else if (metodo == 2) {
                insertion_Sort(subconjunto);
            } else if (metodo == 3) {
                quick_Sort(subconjunto, 0, subconjunto.size() - 1);
            } else if (metodo == 4) {
                merge_Sort(subconjunto, 0, subconjunto.size() - 1);
            } else {
                cout << "Metodo de ordenacao invalido.\n";
                continue;
            }

            cout << "Subconjunto ordenado: ";
            imprimirVetor(subconjunto);
        } else if (opcao == 2) {
            int elemento;
            cout << "Digite o elemento para encontrar seu representante: ";
            cin >> elemento;
            if (elemento < 1 || elemento > n){
                cout << "Elemento invalido.\n";
                continue;
            }else{
            int representante = achar_representante(pai, elemento);
            cout << "O representante do elemento " << elemento << " e " << representante << endl;
            }
        } else if (opcao == 3) {
            int element1, element2;
            cout << "Digite os elementos para fazer uniao: ";
            cin >> element1 >> element2;
            if (element1 < 1 || element1 > n){
                cout << "Elementos invalidos.\n";
                continue;
            }else if (element2 < 1 || element2 > n){
                cout << "Elementos invalidos.\n";
                continue;
            }else{
            unionSets(pai, rank, element1, element2);
            cout << "Uniao feita entre " << element1 << " e " << element2 << endl;
            }
        } else if (opcao == 4) {
            break;
        } else {
            cout << "Opcao invalida, tente novamente.\n";
        }
    }

    return 0;
}

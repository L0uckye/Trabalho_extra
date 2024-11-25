#include <iostream>
#include <vector>
#include <algorithm>
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

// Função para encontrar o representante de um conjunto, sem modificar o conjunto
int achar_representante(const vector<int>& pai, int x) {
    while (pai[x] != x) {
        x = pai[x];  // Subir na árvore até encontrar o representante
    }
    return x;  // Retorna o representante (raiz)
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
// Função para realizar ordenação por meio de bubble_Sort
void bubble_Sort(vector<int>& vec) {
    for (size_t i = 0; i < vec.size() - 1; i++) {
        for (size_t j = 0; j < vec.size() - i - 1; j++) {
            if (vec[j] > vec[j + 1]) {
                swap(vec[j], vec[j + 1]);
            }
        }
    }
}

// Função para realizar ordenação por meio de insertion_Sort 
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

// Função merge para ser usado no algoritmo de merge_Sort 
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

// Função para realizar ordenação por meio de merge_Sort 
void merge_Sort(vector<int>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_Sort(vec, left, mid);
        merge_Sort(vec, mid + 1, right);
        merge(vec, left, mid, right);
    }
}

// Função partition usado no algoritmo de quick_Sort para dar swap nos elementos 
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

// Função para realizar ordenação por meio de quick_Sort
void quick_Sort(vector<int>& vec, int low, int high) {
    if (low < high) {
        int pi = partition(vec, low, high);
        quick_Sort(vec, low, pi - 1);
        quick_Sort(vec, pi + 1, high);
    }
}

// Função para imprimir a ordenação feita
void Printar_Ordenacao(const vector<int>& vec, ofstream& arquivo) {
    for (int el : vec) {
        arquivo << el << " ";
    }
}

// Função para imprimir o representante de um subconjunto (antes da ordenação)
void imprimirRepresentante(const vector<int>& pai, const vector<int>& subconjunto, ofstream& arquivo) {
    // Encontrar o representante do subconjunto
    int representante = achar_representante(pai, subconjunto[0]);  // Encontrar o representante do primeiro elemento

    // Imprimir o representante do subconjunto
    arquivo << "Representante do conjunto: " << representante << endl;
}

// Função para imprimir o vetor pai
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

int main() {
    srand(time(0));  // Inicializar a semente para números aleatórios

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

    // Ler subconjuntos da entrada
    vector<vector<int>> subconjuntos;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<int> subconj;
        int element;
        
        while (ss >> element) {
            subconj.push_back(element);
        }

        subconjuntos.push_back(subconj);

        // União de todos os elementos do subconjunto
        for (size_t i = 1; i < subconj.size(); i++) {
            unionSets(pai, rank, subconj[0], subconj[i]);
        }
    }

    file.close();

    // Abrir arquivo de saída para escrever o resultado
    int N_subconjunto = 1;
    ofstream arquivo("Resultado.txt");
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo Resultado.txt." << endl;
        return 1;
    }

    // Para cada subconjunto, ordenar e escrever no arquivo
    for (const auto& subconj : subconjuntos) {
        vector<int> sorted_subconj = subconj;

        // Mostrar estado antes da ordenação
        arquivo << "Subconjunto "<< to_string(N_subconjunto) << '\n';
        arquivo << "Antes da ordenação: ";
        Printar_Ordenacao(sorted_subconj, arquivo);
        arquivo << endl;

        // Escolher aleatoriamente o algoritmo de ordenação
        int sorteio = rand() % 4;  // Aleatório entre 0 e 3
        string algoritmo;

        switch (sorteio) {
            case 0:
                algoritmo = "bubble_Sort";
                bubble_Sort(sorted_subconj);
                break;
            case 1:
                algoritmo = "insertion_Sort";
                insertion_Sort(sorted_subconj);
                break;
            case 2:
                algoritmo = "merge_Sort";
                merge_Sort(sorted_subconj, 0, sorted_subconj.size() - 1); // Passando os índices corretamente
                break;
            case 3:
                algoritmo = "quick_Sort";
                quick_Sort(sorted_subconj, 0, sorted_subconj.size() - 1); // Passando os índices corretamente
                break;
        }

        // Mostrar o tipo de ordenação
        // Imprimir o representante de cada conjunto
        imprimirRepresentante(pai, sorted_subconj, arquivo);

        arquivo << "Tipo de ordenação: " << algoritmo << endl;

        // Mostrar estado depois da ordenação
        arquivo << "Depois da ordenação: ";
        Printar_Ordenacao(sorted_subconj, arquivo);
        
        arquivo << endl << endl;
        N_subconjunto++;
    }

    // Imprimir o vetor pai(se quiser ver o vetor pai só modificar abaixo de comentário para código o primeiro //)
    //imprimirVetorPai(pai, arquivo);  // Adicionando a impressão do vetor pai

    arquivo.close();
    cout << "Ordenação concluída e resultados gravados no arquivo 'Resultado.txt'." << endl;

    return 0;
}

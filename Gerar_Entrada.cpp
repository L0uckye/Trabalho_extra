#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Função para embaralhar elementos
void shuffle(vector<int>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        size_t j = rand() % vec.size();
        swap(vec[i], vec[j]);
    }
}

// Função para gerar subconjuntos a partir de um vetor embaralhado
vector<vector<int>> Gerar_particao(int n, int k) {
    vector<int> elements(n);
    for (int i = 0; i < n; ++i) {
        elements[i] = i + 1; // Elementos de 1 a n
    }

    shuffle(elements); // Embaralha os elementos para gerar subconjuntos aleatórios

    vector<vector<int>> subsets(k);
    for (int i = 0; i < n; ++i) {
        subsets[i % k].push_back(elements[i]);
    }

    return subsets;
}

int main() {
    srand(time(0)); // Inicializa o gerador de números aleatórios

    // Defina os valores de n de elementos do conjunto e k(número de partições)
    int n = 42;
    int k = 13;


    if (k > n || k < 1) {
        cerr << "Erro: o número de subconjuntos deve ser entre 1 e n." << endl;
        return 1;
    }

    // Gera a partição dos subconjuntos
    vector<vector<int>> particao = Gerar_particao(n, k);

    // Salva os resultados no arquivo entrada.txt
    ofstream arquivo("entrada.txt");
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo para escrita." << endl;
        return 1;
    }

    arquivo << n << endl; // Escreve o tamanho do vetor na primeira linha
    for (const auto& subset : particao) {
        for (size_t i = 0; i < subset.size(); ++i) {
            arquivo << subset[i];
            if (i < subset.size() - 1) {
                arquivo << " "; // Espaço entre elementos do subconjunto
            }
        }
        arquivo << endl; // Nova linha para o próximo subconjunto
    }

    arquivo.close();

    cout << "Arquivo entrada.txt gerado com sucesso!" << endl;

    return 0;
}

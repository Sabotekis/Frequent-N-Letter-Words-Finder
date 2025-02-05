/****************************************************************************************
Pāvels Petrovs, pp23055
Uzrakstīt programmu, kas atrod dotajā teksta failā visbiežāk lietotos vienburtīgos, divburtīgos, ...,  sešpadsmitburtīgos vārdus. 
Programmai vārdi jāsaliek tabulā, kas realizēta kā masīvs no sarakstiem. Saraksta elementam jāsatur vārds, skaits (cik reižu vārds sastapts tekstā) un norāde uz nākošo elementu. 
Par vārdu uzskatīt tikai tos, kas sastāv no burtiem. Vārdi failā atdalīti ar tukšumiem vai pieturas zīmēm.
Programma izveidota: 2024/06/15
*****************************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <algorithm>
using namespace std;

struct WordNode {
    string word;
    int count;
    WordNode* next;
    WordNode(const string& w, int c) : word(w), count(c), next(nullptr) {}
};

class WordTable {
public:
    unordered_map<int, vector<WordNode*>> table; // tabula, kas saista vārdu garumu ar sarakstu no vārdiem

    void add_word(const string& word) {
        int length = word.length(); // iegūt vārda garumu
        auto& nodes = table[length]; // iegūt atsauci uz sarakstu ar konkrētu garuma vārdiem
        for (auto node : nodes) {
            if (node->word == word) { // ja vārds jau ir tabulā, palielināt skaitu
                node->count++;
                return;
            }
        }
        nodes.push_back(new WordNode(word, 1)); // pievienot jaunu vārdu ar skaitu 1
    }

    vector<string> get_most_frequent_words(int length) {
        vector<string> result;
        if (table.find(length) == table.end()) return result; // ja nav vārdu ar šādu garumu, atgriezt tukšu sarakstu
        int max_count = 0;
        for (auto node : table[length]) max_count = max(max_count, node->count); // atrast maksimālo skaitu
        for (auto node : table[length]) if (node->count == max_count) result.push_back(node->word); // pievienot visus vārdus ar maksimālo skaitu
        return result;
    }

    ~WordTable() {
        for (auto& entry : table) for (auto node : entry.second) delete node;
    }
};

// Funkcija, lai attīrītu tekstu, saglabājot tikai burtus un atstarpes
string clean_text(const string& text) {
    string cleaned;
    for (char ch : text) if (isalpha(ch) || isspace(ch)) cleaned += tolower(ch);
    return cleaned;
}

// Funkcija, lai apstrādātu tekstu no faila un pievienotu vārdus tabulai
void process_text(const string& filename, WordTable& word_table) {
    ifstream file(filename);
    if (!file.is_open()) { cerr << "Could not open the file: " << filename << endl; return; }
    stringstream buffer; buffer << file.rdbuf(); file.close();
    istringstream iss(clean_text(buffer.str())); // attīrīt tekstu un ievietot straumē
    string word;
    while (iss >> word) word_table.add_word(word); // pievienot katru vārdu tabulai
}

void print_most_frequent_words(WordTable& word_table) {
    for (int length = 1; length <= 16; ++length) {
        auto most_frequent_words = word_table.get_most_frequent_words(length);
        if (!most_frequent_words.empty()) {
            cout << "Most frequent " << length << "-letter word(s): ";
            for (const auto& word : most_frequent_words) {
                cout << word << " (";
                for (auto node : word_table.table[length]) if (node->word == word) { cout << node->count; break; }
                cout << ") ";
            }
            cout << endl;
        }
    }
}

int main() {
    string filename = "input.txt"; 
    WordTable word_table; 
    process_text(filename, word_table); 
    print_most_frequent_words(word_table); 
    return 0;
}


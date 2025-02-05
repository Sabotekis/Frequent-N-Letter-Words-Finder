/****************************************************************************************
Pāvels Petrovs, pp23055
Uzrakstīt programmu, kas atrod dotajā teksta failā visbiežāk lietotos vienburtīgos, divburtīgos, ...,  sešpadsmitburtīgos vārdus. 
Programmai vārdi jāsaliek tabulā, kas realizēta kā masīvs no sarakstiem. Saraksta elementam jāsatur vārds, skaits (cik reižu vārds sastapts tekstā) un norāde uz nākošo elementu. 
Par vārdu uzskatīt tikai tos, kas sastāv no burtiem. Vārdi failā atdalīti ar tukšumiem vai pieturas zīmēm.
Programma izveidota: 2024/06/15
*****************************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

struct WordInfo {
    string word;
    int count;
    WordInfo* next;
};

void addWord(unordered_map<int, WordInfo*>& wordMap, const string& word) {
    int length = word.length();
    if (length < 1 || length > 16) {
        return;
    }

    WordInfo* wordInfo = wordMap[length];
    while (wordInfo != nullptr) {
        if (wordInfo->word == word) {
            wordInfo->count++;
            return;
        }
        wordInfo = wordInfo->next;
    }

    wordInfo = new WordInfo;
    wordInfo->word = word;
    wordInfo->count = 1;
    wordInfo->next = wordMap[length];
    wordMap[length] = wordInfo;
}

void printWordTable(const unordered_map<int, WordInfo*>& wordMap) {
    vector<int> lengths;
    for (const auto& pair : wordMap) {
        lengths.push_back(pair.first);
    }
    sort(lengths.begin(), lengths.end());

    for (const auto& length : lengths) {
        WordInfo* wordInfo = wordMap.at(length);

        cout << length << "-letter words:" << endl;
        while (wordInfo != nullptr) {
            cout << wordInfo->word << " (" << wordInfo->count << ")" << endl;
            wordInfo = wordInfo->next;
        }

        cout << endl;
    }
}

int main() {
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Failed to open input file." << endl;
        return 1;
    }

    unordered_map<int, WordInfo*> wordMap;

    string word;
    while (inputFile >> word) {
        word.erase(remove_if(word.begin(), word.end(), [](char c) {
            return !isalpha(c);
        }), word.end());

        addWord(wordMap, word);
    }

    printWordTable(wordMap);

    for (auto& pair : wordMap) {
        WordInfo* wordInfo = pair.second;
        while (wordInfo != nullptr) {
            WordInfo* nextWordInfo = wordInfo->next;
            delete wordInfo;
            wordInfo = nextWordInfo;
        }
    }

    return 0;
}
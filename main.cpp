#include "iostream"
#include <stack>
#include <vector>
#include <sstream>
#include "Read.h"
#include "Write.h"

class Node{
public:
    char key;
    std::string value;
    bool visited = false;
    Node* nodes[128];
    Node* previous;
    Node(){
        for (auto & node : nodes){
            node = nullptr;
        }
    }
};

std::vector<std::string> split(const std::string& strReal, char splitter){
    std::vector<std::string> result;
    std::stringstream data(strReal);
    std::string value;
    while (getline(data, value, splitter)){
        result.push_back(value);
    }

    return result;
}

std::string leftTrim(const std::string &leftStr)
{
    size_t start = leftStr.find_first_not_of(" \n\r\t\f\v");
    return (start == std::string::npos) ? "" : leftStr.substr(start);
}

std::string rightTrim(const std::string &rightStr)
{
    size_t end = rightStr.find_last_not_of(" \n\r\t\f\v");
    return (end == std::string::npos) ? "" : rightStr.substr(0, end + 1);
}

std::string trim(const std::string &s) {
    return rightTrim(leftTrim(s));
}

std::string ReadFile(std::string& fileName){
    std::string inputFile = Read::read(fileName);
    return inputFile;
}

bool atLeastOneNonNull(Node* next){
    for (int i = 97; i < 123; i++){
        if (next->nodes[i] != nullptr){
            return true;
        }
    }
    return false;
}

bool moreThanOne(Node* next){
    int count = 0;
    for (auto & node : next->nodes){
        if (node != nullptr){
            count ++;
        }
        if (count > 1){
            return true;
        }
    }
    return false;
}

bool allVisited(Node* next){
    for (int i = 97; i < 123; i++){
        if ((next->nodes[i] != nullptr) && !next->nodes[i]->visited){
            return false;
        }
    }
    return true;
}

std::vector<std::string> requestTheBranch(Node* next, std::string& FirstLevel){
    std::stack<Node*> controlStack;
    std::vector<std::string> result;
    std::string resultStr = FirstLevel;
    controlStack.push(next);
    while (!controlStack.empty()){
        for (int index = 97; index < 123; index++){
            if (controlStack.size() == 1 && allVisited(next)){
                for (int notVisit = 97; notVisit < 123; notVisit++){
                    if (next->nodes[notVisit] != nullptr){
                        next->nodes[notVisit]->visited = false;
                    }
                }
                controlStack.pop();
                break;
            }
            if (next->nodes[index] != nullptr && !next->nodes[index]->visited) {
                if (moreThanOne(next)) {
                    if (controlStack.top() != next){
                        for (int tab = 0; tab < controlStack.size(); tab++){
                            resultStr = "\t" + resultStr;
                        }
                        if (!next->value.empty()){
                            resultStr += "(" + next->value + ")";
                        }
                        result.push_back(resultStr);
                        controlStack.push(next);
                        resultStr = FirstLevel + controlStack.top()->key;
                    }
                    next = next->nodes[index];
                    next->visited = true;
                } else{
                    next = next->nodes[index];
                }
                resultStr += next->key;
                if (!next->value.empty() && !atLeastOneNonNull(next)){
                    for (int tab = 0; tab < controlStack.size(); tab++){
                        resultStr = "\t" + resultStr;
                    }
                    resultStr += "(" + next->value + ")";
                    result.push_back(resultStr);
                    if (controlStack.size() > 1) {
                        resultStr = FirstLevel + controlStack.top()->key;
                    } else {
                        resultStr = FirstLevel;
                    }
                    next = controlStack.top();
                    if (allVisited(next)){
                        for (int notVisit = 97; notVisit < 123; notVisit++){
                            if (next->nodes[notVisit] != nullptr){
                                next->nodes[notVisit]->visited = false;
                            }
                        }
                        controlStack.pop();
                        resultStr = FirstLevel;
                    }
                    if (!controlStack.empty()){
                        next = controlStack.top();
                    }
                } else if (!next->value.empty() && atLeastOneNonNull(next)){
                    std::string myStr = resultStr;
                    for (int tab = 0; tab < controlStack.size(); tab++){
                        resultStr = "\t" + resultStr;
                    }
                    resultStr += "(" + next->value + ")";
                    result.push_back(resultStr);
                    resultStr = myStr;
                }
                break;
            }
        }
    }
    return result;
}

std::string insert(Node**root, std::string k, std::string v){
    std::string result;
    Node* following = *root;
    bool newCreated = false;
    for (char i : k){
        if (following->nodes[i] == nullptr){
            following->nodes[i] = new Node();
            following->nodes[i]->key = i;
            following = following->nodes[i];
            newCreated = true;
        } else{
            following = following->nodes[i];
        }
    }
    if (!newCreated){
        if (following->value == v){
            result += '"' + k + '"' + " already exist\n";
        }else if (following->value.empty()){
            result += '"' + k + '"' + " was added\n";
            following->value = v;
        } else{
            following->value = v;
            result += '"' + k + '"' + " was updated\n";
        }
    } else {
        result += '"' + k + '"' + " was added\n";
        following->value = v;
    }
    return result;
}

std::string search(Node** root, std::string k){
    Node* next = *root;
    std::string result;
    if (next->nodes[k.at(0)] == nullptr){
        result += "\"no record\"\n";
        return result;
    }
    for (char i : k){
        if (next->nodes[i] == nullptr){
            result += "\"incorrect Dothraki word\"\n";
            return result;
        } else{
            next = next->nodes[i];
        }
    }
    if (next->value.empty()){
        result += "\"not enough Dothraki word\"\n";
    } else{
        result += "\"The English equivalent is " + next->value + "\"\n";
    }
    return result;
}

std::string deleteChar(Node**root, std::string k){
    Node* next = *root;
    std::string result;
    if (next->nodes[k.at(0)] == nullptr){
        result += "\"no record\"\n";
        return result;
    }
    for (char i : k){
        if (next->nodes[i] == nullptr){
            result += "\"incorrect Dothraki word\"\n";
            return result;
        } else{
            Node* temp = next;
            next = next->nodes[i];
            next->previous = temp;
        }
    }
    if (next->value.empty()){
        result += "\"not enough Dothraki word\"\n";
    } else{
        for (int i = 0; i < k.length(); i++) {
            if (atLeastOneNonNull(next)){
                if (!next->value.empty()){
                    next->value = "";
                }
                break;
            }
            char key = next->key;
            next = next->previous;
            next->nodes[key] = nullptr;
        }
        result += '"' + k + '"' + " deletion is successful\n";
    }
    return result;
}

std::string list(Node**root){
    std::string result;
    Node* next = *root;
    for (int index = 97; index < 123; ++index) {
        if (next->nodes[index] != nullptr){
            std::string printFirstLevel = "-";
            next = next->nodes[index];
            // For the first level
            if (!moreThanOne(next)){
                while (true){
                    if (moreThanOne(next)){
                        printFirstLevel += next->key;
                        break;
                    } else if (!next->value.empty()){
                        printFirstLevel += next->key;
                        break;
                    } else{
                        printFirstLevel += next->key;
                        for (int i = 97; i < 123; ++i) {
                            if (next->nodes[i] != nullptr){
                                next = next->nodes[i];
                                break;
                            }
                        }
                    }
                }
            } else{
                printFirstLevel += next->key;
            }

            //For the branch
            if (atLeastOneNonNull(next)){
                std::string temp = printFirstLevel;
                if (!next->value.empty()){
                    temp += "(" + next->value + ")";
                }
                result += temp + "\n";
                std::vector<std::string> print = requestTheBranch(next, printFirstLevel);
                for (std::string a : print){
                    result += a + "\n";
                }
                next = *root;
            } else{
                if (!next->value.empty()){
                    printFirstLevel += "(" + next->value + ")";
                }
                result += printFirstLevel + "\n";
                next = *root;
                continue;
            }
        }
    }
    return result;
}

std::string doAll(std::string fileName){
    std::string result;
    Node* root = new Node();
    std::string file = ReadFile(fileName);
    for (std::string operation : split(file, '\n')){
        if (operation.at(0) == 'i'){
            std::string key = split(split(operation, '(').at(1), ',').at(0);
            std::string value = split(split(split(operation, '(').at(1), ',').at(1), ')').at(0);
            result += insert(&root, key, value);
        } else if (operation.at(0) == 's'){
            std::string key = split(split(operation, '(').at(1), ')').at(0);
            result += search(&root, key);
        } else if (operation.at(0) == 'd'){
            std::string key = split(split(operation, '(').at(1), ')').at(0);
            result += deleteChar(&root, key);
        } else if (operation.at(0) == 'l'){
            result += list(&root);
        }
    }
    return trim(result);
}

int main(int argc, char *argv[]){
    Write::write(doAll(argv[1]), argv[2]);
    return 0;
}
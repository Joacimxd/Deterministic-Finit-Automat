#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include <unordered_map>
#include <algorithm>
using namespace std;

struct ColRowHash {
    unordered_map<string, int> cols;
    unordered_map<string, int> rows;
};

struct MatrixHashes {
    vector<vector<string>> matrix;
    ColRowHash hashes;
};



vector<string> read_vector() {
    char buffer[256];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return {};
    }

    string in(buffer);
    if (!in.empty() && in.back() == '\n') {
        in.pop_back();
    }

    vector<string> resultado;
    stringstream ss(in);
    string item;
    while (getline(ss, item, ',')) {
        resultado.push_back(item);
    }

    return resultado;
}

void print_vector(vector<string> vector, int spacing, int biggest_element){
    for(int i = 0; i<vector.size(); i++){
        cout << vector[i] << flush;
        for(int j = 0; j < biggest_element + spacing - vector[i].size(); j++){
            cout << " " << flush;
        }
    }
    cout << endl << flush;
}

bool is_in_vector(vector<string> vec, string str){
    for(string s : vec){
        if(s == str){
            return true;
        }
    }
    return false;
}

void clearScreen() {
    cout << "\033[2J\033[H" << flush;
}

bool accepts_string(MatrixHashes matrix_hashes, vector<string> str, 
    string init_state, vector<string> acc_states){

    int row_idx = matrix_hashes.hashes.rows[init_state];
    int col_idx;
    string row_string = matrix_hashes.matrix[row_idx][0];

    for(string c: str){
        col_idx = matrix_hashes.hashes.cols[c];
        row_string = matrix_hashes.matrix[row_idx][col_idx];
        row_idx = matrix_hashes.hashes.rows[row_string];
    }
    return is_in_vector(acc_states, row_string);
}

ColRowHash add_to_hash(ColRowHash hashes, int index[2], string element){
    if(index[0] == 0 && index[1] == 0){
        return {hashes.cols, hashes.rows};
    }
    else if(index[0] == 0){
        hashes.cols[element] = index[1];
    }
    else if(index[1] == 0){
        hashes.rows[element] = index[0];
    }
    return hashes;
}

MatrixHashes readMatrixHashFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    int rows, cols;
    file >> rows >> cols;

    MatrixHashes matrix_hash;
    matrix_hash.matrix = vector<vector<string>>(rows, vector<string>(cols));

    string curr;
    int index[2];

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file >> curr;
            index[0] = i;
            index[1] = j;
            matrix_hash.matrix[i][j] = curr;
            matrix_hash.hashes = add_to_hash(matrix_hash.hashes, index, curr);
        }
    }

    file.close();
    return matrix_hash;
}

void delete_from_hash(ColRowHash& hashes, string clave, int index[2]) {
    if(index[0] == 0 && index[1] == 0){
        return;
    }
    else if(index[0] == 0){
        hashes.cols.erase(clave);
    }
    else if(index[1] == 0){
        hashes.rows.erase(clave);
    }
}

char getch() {
    char buf = 0;
    struct termios old = {0};
    tcgetattr(STDIN_FILENO, &old);
    struct termios newt = old;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    read(STDIN_FILENO, &buf, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    return buf;
}

void print_matrix(vector<vector<string>> matrix, int spacing, int biggest_element){
    clearScreen();
    for(int i = 0; i<matrix.size(); i++){
        print_vector(matrix[i], spacing, biggest_element);
    }
}

void gotoxy(int x, int y) {
    cout << "\033[" << y << ";" << x << "H" << flush;
}

vector<vector<string>> grow_matrix_col(vector<vector<string>> matrix) {
    for (auto& row : matrix) {
        row.push_back("");  
    }
    return matrix;
}

vector<vector<string>> grow_matrix_row(vector<vector<string>> matrix){
    vector<string> sum_row;
    for(int i = 0; i<matrix[0].size(); i++){
        sum_row.push_back("");
    }
    matrix.push_back(sum_row);

    return matrix;
}

MatrixHashes edit_matrix(){
    int spacing = 2;
    clearScreen();
    int biggest_element = 1;
    MatrixHashes matrix_hashes;
    matrix_hashes.matrix.push_back({""});
    int index[2] = {0,0};
    bool index_changed;

    while (true) {
        char c = getch();
        if (c == 27) {
            char c1 = getch();
            if (c1 == '[') {
                index_changed = true;
                char c2 = getch();
                if (c2 == 'A') {         // ↑
                    index[0] = max(0, index[0]-1);
                } else if (c2 == 'B') {  // ↓
                    index[0]++;
                    matrix_hashes.matrix = grow_matrix_row(matrix_hashes.matrix);
                } else if (c2 == 'C') {  // →
                    index[1]++;
                    matrix_hashes.matrix = grow_matrix_col(matrix_hashes.matrix);
                } else if (c2 == 'D') {  // ←
                    index[1] = max(0, index[1]-1);
                }
                gotoxy(index[1] * (biggest_element + spacing) + 1, index[0] + 1);
                
                print_matrix(matrix_hashes.matrix, spacing, biggest_element);
                gotoxy(index[1] * (biggest_element + spacing) + 1, index[0] + 1);
                
            } else {
                break;
            }
        }else{
            if(index_changed){
                delete_from_hash(matrix_hashes.hashes, matrix_hashes.matrix[index[0]][index[1]], index);
                matrix_hashes.matrix[index[0]][index[1]] = "";
                index_changed = false;
            }
            matrix_hashes.matrix[index[0]][index[1]] += c;
            biggest_element = max(static_cast<int>(matrix_hashes.matrix[index[0]][index[1]].length()), biggest_element);
            matrix_hashes.hashes = add_to_hash(matrix_hashes.hashes, index, 
                matrix_hashes.matrix[index[0]][index[1]]);
            print_matrix(matrix_hashes.matrix, spacing, biggest_element);
            gotoxy(index[1] * (biggest_element + spacing) + 1, index[0] + 1);
        }
    }
    print_matrix(matrix_hashes.matrix, spacing, biggest_element);
    return matrix_hashes;
}

int main() {
    cout << "Read a file or write matrix in editor (r/w): ";
    char read_write;
    cin >> read_write;

    MatrixHashes matrix_hashes;
    if (read_write == 'w') {
        matrix_hashes = edit_matrix();
    } else {
        string file;
        cout << "Introduce the file name: ";
        cin >> file;
        matrix_hashes = readMatrixHashFromFile(file);
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string init_state;
    cout << "Introduce the initial state: ";
    getline(cin, init_state);

    cout << "Introduce the states of acceptance as [1,2,...,n]: " << flush;
    vector<string> acc_states = read_vector();

    while (true) {
        cout << "Introduce the string as [1,2,...,n]: " << flush;
        vector<string> str = read_vector();
        if (accepts_string(matrix_hashes, str, init_state, acc_states)) {
            cout << "It is a valid string\n";
        } else {
            cout << "It is NOT a valid string\n";
        }
    }
}
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
using namespace std;

void clearScreen() {
    cout << "\033[2J\033[H";
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

vector<string> read_vector() {
    vector<string> buffer;
    string word;
    char c;
    while(true){
        while (true) {
            c = getch();
            if (c == ' ' or c == '\n') {
                cout << "  " << flush;
                break;
            } else {
                word.push_back(c);
                cout << c << flush;
            }
        }
        if(c == '\n'){
            cout << "\n" << flush;
            break;
        }
        buffer.push_back(word);
        word = "";
    }

    return buffer;
}

void print_vector(vector<string> vector, int biggest_element){
    for(int i = 0; i<vector.size(); i++){
        cout << vector[i] << flush;
        for(int j = 0; j < biggest_element + 2 - vector[i].size(); j++){
            cout << " " << flush;
        }
    }
    cout << endl;
}

void print_matrix(vector<vector<string>> matrix, int biggest_element){
    clearScreen();
    for(int i = 0; i<matrix.size(); i++){
        print_vector(matrix[i], biggest_element);
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

vector<vector<string>> edit_matrix(int spacing, int biggest_element){
    vector<vector<string>> matrix;
    matrix.push_back({""});
    int index[2] = {0,0};
    string s = "";

    while (true) {
        char c = getch();

        if (c == 27) {
            char c1 = getch();
            matrix[index[0]][index[1]] = s;
            s="";
            if (c1 == '[') {
                char c2 = getch();
                if (c2 == 'A') {         // ↑
                    index[0] = max(0, index[0]-1);
                } else if (c2 == 'B') {  // ↓
                    index[0]++;
                    matrix = grow_matrix_row(matrix);
                } else if (c2 == 'C') {  // →
                    index[1]++;
                    matrix = grow_matrix_col(matrix);
                } else if (c2 == 'D') {  // ←
                    index[1] = max(0, index[1]-1);
                }

                gotoxy(index[1] * (biggest_element + spacing) + 1, index[0] + 1);
            } else {
                break;
            }
        }else{
            s.push_back(c);
        }
    }
    return matrix;
}




int main() {
    int SPACING = 2;
    int BIGGEST_ELEMENT = 5;
    vector<vector<string>> matrix;
    matrix.push_back({"1", "222", "3"});
    matrix.push_back({"1", "2", "33333"});
    print_matrix(matrix, BIGGEST_ELEMENT);
    cout << "\033[A" << flush;
    cout << "\033[A" << flush;
    matrix = edit_matrix(SPACING, BIGGEST_ELEMENT);
    print_matrix(matrix, BIGGEST_ELEMENT);
}

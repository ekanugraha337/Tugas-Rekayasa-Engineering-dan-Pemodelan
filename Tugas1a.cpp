#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath> // Tambahan untuk std::abs() pada tipe double

using namespace std;
typedef vector<vector<double>> Matrix;
// Variabel global
int n = 3;
bool openFile;
string fileString;
double factor;
bool debug = false;
bool printHelp = false;
// string inputFileName = "data.csv"; // Default file name

// Deklarasi fungsi menggunakan vector reference
void printEquation(const vector<double>& eq);
Matrix readMatrix(ifstream& file);
void printMatrix(Matrix result);

int main(int argc, char * argv[]) {
    
    // ==== A R G U M E N T =============================== 
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--debug" || arg == "-d") {
            debug = true;
        }
        else if ((arg == "-f" || arg == "--file") && i + 1 < argc) {
            fileString = argv[i + 1]; // Simpan ke variabel global
            cout << "Open file, " << fileString << "!" << endl;
            i++; // Lewati indeks nama karena sudah diproses
            openFile = true;
        }
        else if (arg == "-h" || arg == "--help") {
            printHelp = true;
            cout << "[-f] / [--file] [\033[3mfile_location\033[0m]\t : specify file location and extension" << endl;
            cout << "[-d] / [--debug] \t \t : activate debug" << endl;
            return 0; // Langsung keluar setelah print help
        }
        else {
            cout << "argument not recognized: " << arg << endl;
        }
    }

    if (!openFile) {
        cout << "Please insert file name location .csv" << endl; 
        return -1;
    }

    // -- open file
    ifstream file(fileString);
    if (!file.is_open()) {
        cout << "Gagal membuka file!" << endl;
        return 1;
    }

    // get Matriks A
    Matrix A = readMatrix(file);
    cout << "Matrix A : " << endl;
    printMatrix(A);

    cout << endl;

    file.close();
    //-------------------------------
    // Get size (row) matrix
    n = A.size() ;

    // print equation per row
    cout << "Equation : " << endl;
    for (int i = 0; i < n; i++) { // access row
        printEquation(A[i]);    
    }
    
    // start calculation by elimination process
    // get factor by compare mat[1][0] / mat[0][0]
    for (int j = 0; j < n; j++) { // 0 + 1, 0 + 2, 1 + 2
        for (int k = j + 1; k < n; k++) {           
            factor = A[j][j] / A[k][j];         // find the factor            
            
            if (debug) {
                cout << "Find factor coefisient row " << j << " / row " << k;
                cout << ", factor : " << factor << endl;
            }

            for (int i = 0; i <= n; i ++) {         // update the row k for every column
                A[k][i] = A[j][i] - (A[k][i] * factor);
                }
        
            if (debug) {
                cout << "update row " << k << " become :";
                for (int i = 0; i <= n; i++) {
                     cout << "[" << A[k][i] << "] ";
                }
                cout << endl << endl;
            }
        }
    }

    if (debug) {
        cout << "final matrix : " << endl;
        for (int i = 0; i < n; i++) { // access baris
            cout << "[\t";
            for (int j = 0; j <= n; j++ ){ // access column
                cout << A[i][j] << "\t";
            }
            cout << "]" << endl;
        }
    }

    if (debug) {
        cout << "final equation : " << endl;
        for (int i = 0; i < n; i++) { // access row
            printEquation(A[i]);    
        }
    }

    vector<double> res(n);
    // get value of the last variable
    // --- Back Substitution ---    
    for (int i = n - 1; i >= 0; i--) {
        res[i] = A[i][n];
        for (int j = i + 1; j < n; j++) {
            res[i] = res[i] - A[i][j] * res[j];
        }
        res[i] = res[i] / A[i][i];
    }

     // --- S H O W    R E S U L T ---
    cout << "Result:" << endl;
    for (int i = 0; i < n; i++) {
        cout << (char) (0x41 + i) << " = " << res[i] << endl;
    }
    // ==========================================================
     
    return 0;
}

// Menggunakan tipe const reference dari vector
void printEquation(const vector<double>& eq) 
{
    for (int j = 0; j <= n; j++) {
        if (eq[j] == 0) continue; // skip excecution
        
        else {
            if (j == 0) {
                // give sign (diubah ke < 0 agar aman untuk desimal)
                if (eq[j] < 0) cout << "-"; 
                
                // give coeffisient (menggunakan std::abs untuk double)
                if (std::abs(eq[j]) == 1); // skip
                else cout << std::abs(eq[j]);

                // give variable 
                cout << "A ";
            }
            else if (j > 0 && j < n) {
                // give sign
                if (eq[j] < 0) cout << "- ";
                else if (eq[j] > 0) cout << "+ ";

                // give coeffisient
                if (std::abs(eq[j]) == 1); // skip
                else cout << std::abs(eq[j]);

                // give variable
                cout << (char) (0x41 + j) << " ";                    
            }
            else if (j == n) {
                cout << "= ";
                // give sign
                if (eq[j] < 0) cout << "-";
                
                cout << std::abs(eq[j]);
            }
        }
    }    
    cout << "\n";
}
Matrix readMatrix(ifstream& file) {
    Matrix tempMatrix;
    string line;
    
    while (getline(file, line)) {
        // Jika bertemu baris kosong, artinya matriks saat ini sudah selesai
        if (line.empty() || line == ",") break; 

        vector<double> row;
        stringstream ss(line);
        string value;

        while (getline(ss, value, ',')) { // ',' adalah delimiter CSV
            if (!value.empty()) {
                row.push_back(stod(value));
            }
        }
        tempMatrix.push_back(row);
    }
    return tempMatrix;
}  

void printMatrix(Matrix result) {
    // print result addition
    int row = result.size();

    int column = result[0].size();

    for (int i = 0; i < row; i++) // access row
    {
        for (int j = 0; j < column; j++) {
            cout << result[i][j] << "\t";
        }
        cout << endl;
    }
}
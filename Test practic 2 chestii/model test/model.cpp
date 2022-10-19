#include <mpi.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <string>

#define MAXCHARACTERSLINE 15

bool isVowel (char x) {
    if (x == 'a' || x == 'e' || x == 'i' || x == 'o' || x == 'u' ||
            x == 'A' || x == 'E' || x == 'I' || x == 'O' || x == 'U')
        return true;

    return false;
}
 
using namespace std;

int main (int argc, char *argv[]) {
    int numtasks, rank;

    MPI_Request request;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int noMappers = 3;
    int noReducers = 2;

    vector<string> lines;

    int recv_num;
    int i, j;

    // Coordinator
    if (rank == 0) {
        
        string file = argv[1];
        cout << file << endl;
        ifstream readFile(file);
        string text;
        getline(readFile, text);
        int numLines = stoi(text);
        while (getline(readFile, text)) {
            // cout << text << endl;
            lines.push_back(text);
        }

        int subtask = (int) numLines / noMappers;

        // Processes 1 - 3: mappers
        for (i = 1; i <= 3; i++) {
            int tasksToSend = subtask;
            if (i == 3) {
                tasksToSend = numLines - 2 * subtask;
            } 
            // Send number of lines
            MPI_Send(&tasksToSend, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            // Send the lines
            for (j = 0; j < tasksToSend; j++) {
                MPI_Send(lines[j + (i - 1) * 3].c_str(), lines[j + (i - 1) * 3].size(), MPI_CHAR, i, 1, MPI_COMM_WORLD);
            }
            
        }

    }

    // Mappers 
    else if (rank >= 1 && rank <= 3) {

        int vowels = 0, consonants = 0;

        int tasksToReceive;
        MPI_Recv(&tasksToReceive, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        // Receive lines from coordinator
        string line;
        for (j = 0; j < tasksToReceive; j++) {
            MPI_Probe(0, 1, MPI_COMM_WORLD, &status);
            int size = status._ucount;
            char *buf = new char[size];
            MPI_Recv(buf, size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

            line.assign(buf, size);

            // Compute number of vowels and consonants from all received lines
            for (i = 0; i < size; i++) {
                if (isVowel(buf[i])) {
                    vowels++;
                } else {
                    consonants++;
                }
            }

            cout << "[" << rank << "] " << line << endl;
        }

        cout << "[" << rank << "] v=" << vowels << " c=" << consonants << endl;

        // Send vowels to process 4 (Reducer)
        MPI_Send(&vowels, 1, MPI_INT, 4, 1, MPI_COMM_WORLD);
        // Send consonants to process 5 (Reducer)
        MPI_Send(&consonants, 1, MPI_INT, 5, 1, MPI_COMM_WORLD);
    }

    // Reducers
    else if (rank == 4 || rank == 5) {
       
        int letters, total = 0;
        for (i = 1; i <= 3; i++) {
            MPI_Recv(&letters, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
            total += letters;
        }

        if (rank == 4) {
            cout << "[4] Vowels: " << total << endl;
        }

        if (rank == 5) {
            cout << "[5] Consonants: " << total << endl;
        }
    }

    MPI_Finalize();

    return 0;
}

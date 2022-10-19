#include <mpi.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>

using namespace std;

void printLog(int sender, int receiver) {
    cout << "M(" << sender << "," << receiver << ")" << endl;
}

void broadcastLog(int source, vector<int> cluster) {
    for (auto i : cluster) {
        printLog(source, i);
    }
}

void notifyWorkers(int info, vector<int> cluster, int source) {
    for (auto i : cluster) {
        MPI_Send(&info, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        printLog(source, i);
    }
}

void notifyWorkersVec(vector<int> info, vector<int> cluster, int source) {
    int size = cluster.size();
    for (auto i : cluster) {
        MPI_Send(&cluster[0], size, MPI_INT, i, 1, MPI_COMM_WORLD);
        printLog(source, i);
    }
}


int main (int argc, char *argv[])
{
    int numtasks, rank;
    MPI_Status status;

    string testNo = "1";
    string path = "../tests/test" + testNo + "/";

    // Number of clusters
    int clusters = 3;
    int myClusterSize;
    vector<int> myCluster;
    // The clusteres to be populated for final topology
    vector<int> cluster0, cluster1, cluster2;
    // The rank of the coordinator for each process' cluster
    int coordinatorRank;

    // Initialize MPI program. Arguments are given from command line
    MPI_Init(&argc, &argv);

    // MPI_COMM_WORLD -> communicator, numtasks -> number of tasks to perform in the communicator
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    // sets the process rank in the communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // TOPOLOGY

    int error = stoi(argv[2]);

    // Coordinators read their cluster info from files
    if (rank == 0 || rank == 1 || rank == 2) {
        
        // ifstream readFile(path + "cluster" + std::to_string(rank) + ".txt");
        ifstream readFile("cluster" + std::to_string(rank) + ".txt");
        string text;
        getline(readFile, text);
        myClusterSize = stoi(text);
        for (int i = 0; i < myClusterSize; i++) {
            getline(readFile, text);
            myCluster.push_back(stoi(text));
        }

        // We don't add coordinators' ranks to the clusters. They are already known by the cluster name
        switch (rank) {
        case 0:
            cluster0 = myCluster;
            coordinatorRank = 0;
            break;
        case 1:
            cluster1 = myCluster;
            coordinatorRank = 1;
            break;
        case 2:
            cluster2 = myCluster;
            coordinatorRank = 2;
            break;
        default:
            break;
        }
    }


    // Inform workers of their coordinator
    // Send
    if (rank == 0) {
        notifyWorkers(0, cluster0, 0);
    }
    if (rank == 1) {
        notifyWorkers(1, cluster1, 1);
    }
    if (rank == 2) {
        notifyWorkers(2, cluster2, 2);
    }
    // Receive
    if (rank > 2) {
        MPI_Recv(&coordinatorRank, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
    }

    // Coordinators inform each other about their clusters

    int size;
    for (int i = 0; i < 3; i++) {

        vector<int> currCluster;
        switch (i) {
        case 0:
            currCluster = cluster0;
            break;
        case 1:
            currCluster = cluster1;
            break;
        case 2:
            currCluster = cluster2;
            break;
        default:
            break;
        }

        // Send cluster_i to coordinators from clusters {1,2,3} - {i}
        // Receive and populate cluster_i
        for (int j = 0; j < 3; j++) {

            if (error == 0 || !((i == 1 && j == 0) || (i == 0 && j == 1))) {
                // Send
                if (rank == i && j != i) {
                    size = currCluster.size();
                    MPI_Send(&size, 1, MPI_INT, j, 1, MPI_COMM_WORLD);
                    printLog(i, j);
                    MPI_Send(&currCluster[0], size, MPI_INT, j, 1, MPI_COMM_WORLD);
                    printLog(i, j);
                }
                // Receive
                if (rank == j && j != i) {
                    MPI_Recv(&size, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
                    currCluster.resize(size);
                    MPI_Recv(&currCluster[0], size, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
                    switch (i) {
                    case 0:
                        cluster0 = currCluster;
                        break;
                    case 1:
                        cluster1 = currCluster;
                        break;
                    case 2:
                        cluster2 = currCluster;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    
    // bonus case -> no connection between coordinators 0 and 1
    if (error == 1) {
        int size;
        // 2 sends info about 1 to 0
        if (rank == 2) {
            size = cluster1.size();
            MPI_Send(&size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            printLog(2, 0);
            MPI_Send(&cluster1[0], size, MPI_INT, 0, 1, MPI_COMM_WORLD);
            printLog(2, 0);
        }
        // 0 receives info about 1 from 2
        if (rank == 0) {
            MPI_Recv(&size, 1, MPI_INT, 2, 1, MPI_COMM_WORLD, &status);
            cluster1.resize(size);
            MPI_Recv(&cluster1[0], size, MPI_INT, 2, 1, MPI_COMM_WORLD, &status);
        }

        // 2 sends info about 0 to 1
        if (rank == 2) {
            size = cluster0.size();
            MPI_Send(&size, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
            printLog(2, 1);
            MPI_Send(&cluster0[0], size, MPI_INT, 1, 1, MPI_COMM_WORLD);
            printLog(2, 1);
        }
        // 1 receives info about 0 from 2
        if (rank == 1) {
            MPI_Recv(&size, 1, MPI_INT, 2, 1, MPI_COMM_WORLD, &status);
            cluster0.resize(size);
            MPI_Recv(&cluster0[0], size, MPI_INT, 2, 1, MPI_COMM_WORLD, &status);
        }
    }

    // Split clusters into subcommunicators based on their coordinator; colour = coordinator rank (0 / 1 / 2)
    int new_numtasks, new_rank;
    MPI_Comm custom_group;
    MPI_Comm_split(MPI_COMM_WORLD, coordinatorRank, 0, &custom_group);
    MPI_Comm_size(custom_group, &new_numtasks);   // Total number of processes.
    MPI_Comm_rank(custom_group, &new_rank);       // The current process ID / Rank.

    // Clusters inform their workers about the other clusters
    int s;

    // Information regarding cluster0 is broadcast in every subcommunicator by the new root
    s = cluster0.size();
    MPI_Bcast(&s, 1, MPI_INT, 0, custom_group);
    broadcastLog(0, cluster0);
    cluster0.resize(s);
    MPI_Bcast(&cluster0[0], s, MPI_INT, 0, custom_group);
    broadcastLog(0, cluster0);

    // Information regarding cluster1 is broadcast in every subcommunicator by the new root
    s = cluster1.size();
    MPI_Bcast(&s, 1, MPI_INT, 0, custom_group);
    broadcastLog(1, cluster1);
    cluster1.resize(s);
    MPI_Bcast(&cluster1[0], s, MPI_INT, 0, custom_group);
    broadcastLog(1, cluster1);

    // Information regarding cluster2 is broadcast in every subcommunicator by the new root
    s = cluster2.size();
    MPI_Bcast(&s, 1, MPI_INT, 0, custom_group);
    broadcastLog(2, cluster2);
    cluster2.resize(s);
    MPI_Bcast(&cluster2[0], s, MPI_INT, 0, custom_group);
    broadcastLog(2, cluster2);

    // Coordinators successfully informed their workers about all of the clusters

    MPI_Barrier(MPI_COMM_WORLD);

    // Print topology output
    cout << rank << " -> ";
    cout << "0:";
    for (int i = 0; i < cluster0.size(); i++) {
        cout << cluster0[i];
        if (i < cluster0.size() - 1)
            cout << ",";
    }
    cout << " 1:";
    for (int i = 0; i < cluster1.size(); i++) {
        cout << cluster1[i];
        if (i < cluster1.size() - 1)
            cout << ",";
    }
    cout << " 2:";
    for (int i = 0; i < cluster2.size(); i++) {
        cout << cluster2[i];
        if (i < cluster2.size() - 1)
            cout << ",";
    }
    cout << endl;

    MPI_Barrier(MPI_COMM_WORLD);

    // CALCULATIONS

    int n;
    int p0, p1, p2;
    vector<int> v, v2;

    if (rank == 0) {

        // Read vector size from command line
        n = stoi(argv[1]);

        // Assign vector values
        for (int i = 0; i < n; i++) {
            v.push_back(i);
        }

        // Check percentages in order to distribute the vector to the workers
        int total = cluster0.size() + cluster1.size() + cluster2.size();
        p0 = (int) n * cluster0.size() / total;
        p1 = (int) n * cluster1.size() / total;
        p2 = n - p0 - p1;

        // Send 2nd part of vector to cluster1
        if (error == 0) {
            MPI_Send(&p1, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
            printLog(0, 1);
            MPI_Send(&v[p0], p1, MPI_INT, 1, 1, MPI_COMM_WORLD);
            printLog(0, 1);
        } 
        // Send 2nd part of vector (info about cluster1) to cluster 2
        else {
            MPI_Send(&p1, 1, MPI_INT, 2, 1, MPI_COMM_WORLD);
            printLog(0, 2);
            MPI_Send(&v[p0], p1, MPI_INT, 2, 1, MPI_COMM_WORLD);
            printLog(0, 2);
        }
    
        // Send 3rd part of vector to cluster2
        MPI_Send(&p2, 1, MPI_INT, 2, 1, MPI_COMM_WORLD);
        printLog(0, 2);
        MPI_Send(&v[p0 + p1], p2, MPI_INT, 2, 1, MPI_COMM_WORLD);
        printLog(0, 2);
    }
    
    if (error == 1 && rank == 2) {
        // Receive info about cluster 1 from cluster 0 -> send it to cluster 1
        MPI_Recv(&p1, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        v.resize(p1);
        MPI_Recv(&v[0], p1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        MPI_Send(&p1, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        printLog(2, 1);
        MPI_Send(&v[0], p1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        printLog(2, 1);
    }
    if (rank == 2) {
        // Coordinator 2 will not have the entire vector, only the p2 size
        MPI_Recv(&p2, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        v.resize(p2);
        MPI_Recv(&v[0], p2, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }

    if (error == 0 && rank == 1) {
        // Coordinator 1 will not have the entire vector, only the p1 size
        MPI_Recv(&p1, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        v.resize(p1);
        MPI_Recv(&v[0], p1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }

    if (error == 1 && rank == 1) {
        MPI_Recv(&p1, 1, MPI_INT, 2, 1, MPI_COMM_WORLD, &status);
        v.resize(p1);
        MPI_Recv(&v[0], p1, MPI_INT, 2, 1, MPI_COMM_WORLD, &status);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Coordinators send parts of their parts from the vector to their clusters
    int no; 

    for (int k = 0; k < 3; k++) {

        vector<int> currCluster;
        int p;
        switch (k)
        {
        case 0:
            currCluster = cluster0;
            p = p0;
            break;
        case 1:
            currCluster = cluster1;
            p = p1;
            break;
        case 2:
            currCluster = cluster2;
            p = p2;
            break;
        default:
            break;
        }

        if (rank == k) {
            no = (int) p / currCluster.size(); 
            int sendcounts = no;
            for (int i = 0; i < currCluster.size(); i++) {
                if (i == currCluster.size() - 1) {
                    sendcounts = p - no * (currCluster.size() - 1);
                }
                MPI_Send(&sendcounts, 1, MPI_INT, currCluster[i], 1, MPI_COMM_WORLD);
                printLog(k, currCluster[i]);
                MPI_Send(&v[no * i], sendcounts, MPI_INT, currCluster[i], 1, MPI_COMM_WORLD);
                printLog(k, currCluster[i]);
            }
        }

        if (coordinatorRank == k && rank != k) {
            MPI_Recv(&no, 1, MPI_INT, k, 1, MPI_COMM_WORLD, &status);
            v.resize(no);
            MPI_Recv(&v[0], no, MPI_INT, k, 1, MPI_COMM_WORLD, &status);
        }
    }
    

    // Do *2 on received vector
    if (rank > 2) {
        for (auto i : v) {
            v2.push_back(i * 2);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Send new vector to their coordinators
    if (rank > 2) {
        int s = v2.size();
        MPI_Send(&s, 1, MPI_INT, coordinatorRank, 1, MPI_COMM_WORLD);
        printLog(rank, coordinatorRank);
        MPI_Send(&v2[0], s, MPI_INT, coordinatorRank, 1, MPI_COMM_WORLD);
        printLog(rank, coordinatorRank);
    }

    // Coordinators receive the new vector
    if (rank < 3) {

        vector<int> currCluster;
        switch (rank)
        {
        case 0:
            currCluster = cluster0;
            break;
        case 1:
            currCluster = cluster1;
            break;
        case 2:
            currCluster = cluster2;
            break;
        default:
            break;
        }

        for (int i = 0; i < currCluster.size(); i++) {
            int s;
            MPI_Recv(&s, 1, MPI_INT, currCluster[i], 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&v[no * i], s, MPI_INT, currCluster[i], 1, MPI_COMM_WORLD, &status);
        }
    }
    

    // Coordinators send new vector to process 0
    // bonus conditions -> with error == 1
    if (rank == 1 && error == 1) {
        // Send info to process 2
        int s = v.size();
        MPI_Send(&s, 1, MPI_INT, 2, 1, MPI_COMM_WORLD);
        printLog(rank, 2);
        MPI_Send(&v[0], s, MPI_INT, 2, 1, MPI_COMM_WORLD);
        printLog(rank, 2);
    }

    if (rank == 2 && error == 1) {
        vector<int> aux;
        // Receive from process 1
        MPI_Recv(&s, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
        aux.resize(s);
        MPI_Recv(&aux[0], s, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);

        // Send info about process 1 to process 0
        MPI_Send(&s, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        printLog(rank, 0);
        MPI_Send(&aux[0], s, MPI_INT, 0, 2, MPI_COMM_WORLD);
        printLog(rank, 0);
    }

    if ((rank == 1 && error == 0) || rank == 2) {
        int s = v.size();
        MPI_Send(&s, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        printLog(rank, 0);
        MPI_Send(&v[0], s, MPI_INT, 0, 1, MPI_COMM_WORLD);
        printLog(rank, 0);
    }

    if (rank == 0) {
        int s;
        // Receive from coordinator 1
        if (error == 0) {
            MPI_Recv(&s, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&v[p0], s, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
        } else {
            MPI_Recv(&s, 1, MPI_INT, 2, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&v[p0], s, MPI_INT, 2, 2, MPI_COMM_WORLD, &status);
        }

        // Receive from coordinator 2
        MPI_Recv(&s, 1, MPI_INT, 2, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&v[p0 + p1], s, MPI_INT, 2, 1, MPI_COMM_WORLD, &status);

        cout << "Rezultat: ";
        for (auto i : v) {
            cout << i << " ";
        }
    }


    // BONUS
    // Added conditions with errors == 1


    // Finishes MPI program
    MPI_Finalize();

}

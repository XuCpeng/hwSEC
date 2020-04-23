#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <deque>

using namespace std;

int n = 0;
unsigned int mx = 0;
const int MaxSize = 280000;
unsigned int stack[10];

static deque<unsigned int> Q[3000000];
static int visited[MaxSize] = {0};
static bool inStack[MaxSize] = {false};
static struct Account {
    unsigned int x;
    unsigned int y;
} num[MaxSize];

static struct ArcNode {
    unsigned int adjVex;
    ArcNode *next;
} adjList2[MaxSize];


void SplitString(string &s, vector<string> &v, const string &c) {
    int pos2;
    s += c;
    int size = s.size();
    for (int i = 0; i < size; i++) {
        pos2 = s.find(c, i);
        if (pos2 < size) {
            v.push_back(s.substr(i, pos2 - i));
            i = pos2 + c.size() - 1;
        }
    }
}

void readTxt(const string &file) {

    ifstream in(file, ios::in);
    if (!in.is_open()) {
        cout << "Error: opening file fail" << endl;
        exit(1);
    }
    string s;

    while (getline(in, s)) {
        vector<string> v;
        SplitString(s, v, ",");
        num[n].x = strtoul((const char *) v[0].c_str(), nullptr, 10);
        num[n].y = strtoul((const char *) v[1].c_str(), nullptr, 10);
        if (num[n].x > mx) {
            mx = num[n].x;
        }
        n++;
    }
    in.close();
}

void WriteFile(ofstream &outFile, deque<unsigned int> q, int i) {
    for (int j = 0; j < i - 1; ++j) {
        outFile << q[j] << ",";
    }
    outFile << q[i - 1] << endl;
}

void DeOrder(deque<unsigned int> q[], int count) {
    string filename = "/projects/student/result.txt";
    ofstream outFile;
    outFile.open(filename);
    outFile << count << endl;

#pragma omp parallel for num_threads(5) private(i, j)
    for (int i = 3; i <= 7; ++i) {
        for (int j = 1; j <= count; ++j) {
            if (q[j].size() == i) {
                WriteFile(outFile, q[j], i);
            }
        }
    }
    outFile.close();
}

void DFS(unsigned int v, int &top, int &count) {
    stack[++top] = v;
    inStack[v] = true;
    ArcNode *p = new ArcNode;
    p = adjList2[v].next;
    if (top < 7) {
        while (p != nullptr) {
            if (!visited[p->adjVex]) {
                if (!inStack[p->adjVex]) {
                    DFS(p->adjVex, top, count);
                } else if (stack[0] == p->adjVex && top >= 2) {
                    count++;
                    for (int j = 0; j <= top; j++) {
                        Q[count].push_back(stack[j]);
                    }
                }
            }
            p = p->next;
        }
        delete p;
    }
    inStack[stack[top--]] = false;
}

void CheckCircle() {
    int count = 0;
    int top = -1;
    ArcNode *p;
#pragma omp parallel for num_threads(5) private(i)
    for (int i = 0; i <= mx; i++) {
        if (adjList2[i].next != nullptr) {
            stack[++top] = i;
            inStack[i] = true;
            p = adjList2[i].next;
            while (p != nullptr) {
                if (!visited[p->adjVex]) {
                    DFS(p->adjVex, top, count);
                }
                p = p->next;
            }
            inStack[stack[top--]] = false;
            visited[i] = 1;
        }
    }

    DeOrder(Q, count);
}


int main() {
    string file = "/data/test_data.txt";
    readTxt(file);
#pragma omp parallel for num_threads(5) private(k)
    for (int k = 0; k < n; k++) {
        auto *s = new ArcNode;
        s->adjVex = num[k].y;
        ArcNode *p = &adjList2[num[k].x];
        while (p->next != nullptr && p->next->adjVex < s->adjVex) {
            p = p->next;
        }
        s->next = p->next;
        p->next = s;
    }
    CheckCircle();
    return 0;
}

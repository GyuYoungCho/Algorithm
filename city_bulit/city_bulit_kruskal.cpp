#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <list>
#include <string>
using namespace std;

#define MAX 1000000

struct Node {
    int cityNo = 0;
    string cityName;
    int cityPop = 0;

    Node* prev = NULL;
    Node* next = NULL;
};
Node create_Node(int num, string name, int pop) {
    Node temp;
    temp.cityNo = num;
    temp.cityName = name;
    temp.cityPop = pop;

    temp.prev = NULL;
    temp.next = NULL;
    return temp;
}

vector<Node> CityList;
vector<pair<int, pair<int, int>>> RoadList;
int RoadList_index = 0;
bool terminate_Flag = false;
bool repeat = false;

struct Graph {
    vector<Node> City[MAX];
    int groupID[MAX];
    vector<pair<int, int>> Road[MAX];
    int groupSize[MAX];
    int total_count = 0;

    void mergeGroup(int master, int slave, int weight) {
        City[master].push_back(City[slave].front());
        Road[master].push_back(make_pair(slave, weight));
        int master_leader = groupID[master];
        int slave_leader = groupID[slave];
        if (master != master_leader)
            groupSize[master_leader] += groupSize[slave_leader];
        else
            groupSize[master] += groupSize[slave_leader];
        for (int i = 0; i < (int)CityList.size(); i++) {
            if (groupID[CityList[i].cityNo] == slave_leader || groupID[CityList[i].cityNo] == master_leader) {
                groupID[CityList[i].cityNo] = groupID[master_leader];
                groupSize[CityList[i].cityNo] = groupSize[master_leader];
            }
        }
    }
    void N(int cityNum, int k) {
        int leader = groupID[cityNum];
        string result;
        if (City[leader].size() <= (int)k)        // 찾으려는 순서가 범위를 벗어나면
            cout << "no exist\n";
        else                                // k번째 도시 찾기
            cout << City[leader][k].cityNo << ' ' << City[leader][k].cityName << '\n';
    }

    void L(int cityNum) {
        int leader = groupID[cityNum];
        int count = 0;
        for (int i = 0; i < (int)CityList.size(); i++) {
            if (groupID[CityList[i].cityNo] == leader)
                count++;
        }
        cout << count << '\n';
    }
    string I() {
        int city1 = RoadList[RoadList_index].second.first;
        int city2 = RoadList[RoadList_index].second.second;
        int weight = RoadList[RoadList_index].first;
        int total = 0;
        int getweight = weight;
        string result;
        if (groupID[city1] == groupID[city2])
            result = "not union\n";
        else {
            int master, slave;
            if (groupSize[city1] < groupSize[city2]) {
                getweight = 0;
                master = city2;
                slave = city1;
            }
            else if (groupSize[city1] > groupSize[city2]) {
                master = city1;
                slave = city2;
            }
            else {
                if (groupID[city1] < groupID[city2]) {
                    master = city1;
                    slave = city2;
                }
                else {
                    master = city2;
                    slave = city1;
                }
            }
            mergeGroup(master, slave, weight);
            total += getweight;
            result = to_string(groupID[master]) + ' ' + to_string(groupSize[master]) + '\n';
        }
        RoadList_index++;
        if (groupSize[CityList[0].cityNo] == CityList.size())
            terminate_Flag = true;
        return result;
    }
    void F(int num1, int num2) {
        if (groupID[num1] == groupID[num2])
            cout << "True " << groupID[num1] << '\n';
        else
            cout << "False " << groupID[num1] << ' ' << groupID[num2] << '\n';
    }
    void W(int num1) {
        long long result = 0;
        int leader = groupID[num1];

        for (int i = 0; i < (int)CityList.size(); i++) {
            if (groupID[CityList[i].cityNo] == leader) {
                int temp = CityList[i].cityNo;
                for (int j = 0; j < Road[temp].size(); j++)
                    result += Road[temp][j].second;
            }
        }
        cout << result << '\n';
    }
    void Q() {
        // MST가 완성될 때 까지 가중치-유니온 휴리스틱 적용
        while (RoadList_index < (int)RoadList.size() && !terminate_Flag)
            I();
        cout << groupID[CityList[0].cityNo] << ' ';
        W(CityList[0].cityNo);
    }
};

/*
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    // input.txt 파일 열어 내용 읽기
    ifstream read;
    read.open("input.txt");

    // 도시, 도로, 질의 수 읽기
    int Cities, Roads, Questions;
    read >> Cities >> Roads >> Questions;

    // 도시 정보 읽기
    Graph Map;
    int num, pop;
    string name;
    for (int i = 0; i < Cities; i++) {
        read >> num >> name >> pop;
        CityList.push_back(create_Node(num, name, pop));
        Map.City[num].push_back(create_Node(num, name, pop));
        Map.groupID[num] = num;
        Map.groupSize[num] = 1;
    }

    // 도로 정보 읽기
    int from, to, weight;
    for (int i = 0; i < Roads; i++) {
        read >> from >> to >> weight;
        RoadList.push_back(make_pair(weight, make_pair(from, to)));
    }
    sort(RoadList.begin(), RoadList.end());

    // 질의 정보 읽기
    char question;
    int first, second;
    for (int i = 0; i < Questions; i++) {
        read >> question;
        switch (question) {
        case 'N':
            read >> first >> second;
            Map.N(first, second);
            break;
        case 'L':
            read >> first;
            Map.L(first);
            break;
        case 'I':
            if (!terminate_Flag) cout << Map.I();
            else cout << Map.I();
            break;
        case 'F':
            read >> first >> second;
            Map.F(first, second);
            break;
        case 'W':
            read >> first;
            Map.W(first);
            break;
        case 'Q':
            Map.Q();
            break;
        }
        if (terminate_Flag & ((question == 'Q') | (question == 'I')))
            break;
    }
    return 0;
}*/
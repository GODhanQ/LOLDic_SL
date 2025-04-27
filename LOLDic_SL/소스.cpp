#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#define NOMINMAX
#include <Windows.h>
using namespace std;

const WORD COLOR_DEFAULT{ 0x07 };
const WORD COLOR_YELLOW{ 0x0E };
static void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

struct Node
{
    string name, position;
    int hp{}, mp{}, speed{}, range{};
    Node* next{ nullptr };

    Node(const string n, int h, int m, int s, int r, const string p)
    : name(n), position(p), hp(h), mp(m), speed(s), range(r) {}
    ~Node() {}

    void display() const {
        setConsoleColor(COLOR_YELLOW);
        cout << setw(8) << "Name: ";
        setConsoleColor(COLOR_DEFAULT);
        cout << setw(10) << name << " | "
            << "HP: " << setw(5) << hp << " | "
            << "MP: " << setw(5) << mp << " | "
            << "Speed: " << setw(5) << speed << " | "
            << "Range: " << setw(5) << range << " | "
            << "Position: " << setw(8) << position << "\n";
    }
};
class LinkedList
{
    Node* head;

public:
    int Size{};

    LinkedList() : head(nullptr) {}
    ~LinkedList() {}
    
    void append(string name, int hp, int mp, int speed, int range, string position) {
        Node* newNode = new Node(name, hp, mp, speed, range, position);
        Node* current{ head };
        Node* prdv{ nullptr };

        if (head == nullptr) {
            head = newNode;
            Size++;
            return;
        }
        while (current) {
            if (newNode->hp >= current->hp) {
                if (prdv == nullptr) {
                    newNode->next = current;
                    head = newNode;
                    Size++;
                    return;
                }
                newNode->next = current;
                prdv->next = newNode;
                Size++;
                return;
            }
            prdv = current;
            current = current->next;
        }
        prdv->next = newNode;
        Size++;
    }

    void changehead(Node* p) {
        head = p;
    }

    Node* Head() const {
        return head;
    }

    int size() const {
        return Size;
    }

    // 병합 정렬 메인 함수
    Node* mergeSort(Node* head) {
        if (!head || !head->next) {
            return head;
        }
        Node* secondHalf = splitList(head);
        Node* sortedLeft = mergeSort(head);
        Node* sortedRight = mergeSort(secondHalf);
        return mergeSortedLists(sortedLeft, sortedRight);
    }
private:
    // 리스트를 반으로 나누는 함수
    Node* splitList(Node* head) {
        if (!head || !head->next) {
            return nullptr;
        }

        Node* slow = head;
        Node* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        Node* secondHalf = slow->next;
        slow->next = nullptr;
        return secondHalf;
    }

    // 체력을 기준으로 내림차순으로 병합하는 함수
    Node* mergeSortedLists(Node* left, Node* right) {
        Node dummyHead("", 0, 0, 0, 0, "");
        Node* tail = &dummyHead;

        while (left && right) {
            if (left->hp >= right->hp) { // 체력이 높은 순으로 정렬 (내림차순)
                tail->next = left;
                left = left->next;
            }
            else {
                tail->next = right;
                right = right->next;
            }
            tail = tail->next;
        }
        // left나 right 둘중 하나가 nullptr이 된다면 남아있는 한 쪽은 이미 정렬된 일부분이기에 추가적 조작이 필요 없음.

        if (left) {     // left가 nullptr이 아니라면. = 남아있는 부분이라면.
            tail->next = left;
        }
        if (right) {
            tail->next = right;
        }

        return dummyHead.next;
    }

};

void InSertTestData();
int ExcuteCommand(int CommandNum);

void ShowMenu();
static Node* Search_SL(const string name);
static void InSert_SL(const string name, int hp, int mp, int speed, int range, const string position);
static int Delete_SL(const string name);
static int DeleteAll_SL(const string pos);
static void PrintAll_SL();
static void FindMaxHp_SL();
static void SortByHp_SL(int start, int end);

constexpr int Capacity{ 200 };
LinkedList LOLDic;

int main()
{
    cout << "이 코드는 사전 데이터를 ifstream으로 입력받습니다.\n";
    cout << "리소스 파일에 testdata.txt를 넣어주세요.\n";
    InSertTestData();

    while (true)
    {
        int CommandNum{};

        ShowMenu();
        if (!(cin >> CommandNum))
        {
            system("cls");
            cout << "\n잘못된 입력입니다. 숫자를 입력하세요." << endl;
            cin.clear();  // 입력 스트림 플래그 초기화
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 입력 버퍼 비우기
            continue;
        }

        if (CommandNum == 0)
            break;

        if (ExcuteCommand(CommandNum) == 1) {
            system("cls");
            cout << "잘못된 입력입니다. 다시 입력해주세요. " << endl;
            continue;
        }
    }
}

void ShowMenu()
{
    cout << "--------" << "**Menu**" << "--------" << endl;
    cout << "\t" << "1. Search" << endl;
    cout << "\t" << "2. Insert" << endl;
    cout << "\t" << "3. Delete" << endl;
    cout << "\t" << "4. DeleteAll" << endl;
    cout << "\t" << "5. PrintAll" << endl;
    cout << "\t" << "6. FindMaxHp" << endl;
    cout << "\t" << "7. SortByHp" << endl;
    cout << "0을 누르면 종료됩니다." << endl;
    cout << "커멘드 번호를 입력해주세요. : ";
}
int ExcuteCommand(int CommandNum)
{
    if (CommandNum == 1) {
        system("cls");
        string Sname;
        Node* node{};

        cout << "검색할 챔피언의 이름을 입력해주세요. : ";
        cin >> noskipws >> Sname >> skipws;

        node = Search_SL(Sname);
        if (node != nullptr) {
            system("cls");
            node->display();
        }
        else {
            system("cls");
            cout << "관련 항목을 찾지 못했습니다.\n";
        }
        return 0;
    }
    else if (CommandNum == 2) {
        system("cls");
        int hp{}, mp{}, speed{}, range{};
        string name, position;

        while (true) {
            cout << "추가할 챔피언 정보를 입력해주세요. " << endl;
            cout << "주의 : 입력 순서는 이름, 체력, 마나, 속도, 사거리, 포지션 입니다. " << endl;
            cout << "입력이 부족하다면 계속 입력을 기다리기에 꼭 6개의 입력을 해주세요." << endl;
            
            if (!(cin >> name >> hp >> mp >> speed >> range >> position)) {
                system("cls");
                cout << "입력에 오류가 있습니다. 다시 입력해주세요." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (nullptr != Search_SL(name)) {
                system("cls");
                cout << "중복되는 이름이 있습니다. 다시 입력해주세요." << endl;
                continue;
            }

            break;
        }

        InSert_SL(name, hp, mp, speed, range, position);
        system("cls");
        return 0;
    }
    else if (CommandNum == 3) {
        system("cls");
        string Dname;
        cout << "삭제할 챔피언의 이름을 입력해주세요. : ";
        cin >> Dname;

        Delete_SL(Dname);
        return 0;
    }
    else if (CommandNum == 4) {
        system("cls");
        string Pos;
        bool flag{ false };
        cout << "삭제할 포지션을 입력해주세요. : ";
        cin >> Pos;

        DeleteAll_SL(Pos);
        return 0;
    }
    else if (CommandNum == 5) {
        system("cls");

        PrintAll_SL();
        cout << "총 " << LOLDic.size() << "개의 항목이 있습니다.\n";
        cout << "\n\n";
        return 0;
    }
    else if (CommandNum == 6) {
        system("cls");

        FindMaxHp_SL();
        cout << "\n\n";
        return 0;
    }
    else if (CommandNum == 7) {
        system("cls");

        SortByHp_SL(0, Capacity - 1);
        cout << "내림차순 정렬되었습니다.\n\n";
        return 0;
    }
    else {
        return 1;
    }
}

static Node* Search_SL(const string name)
{
    Node* current{ LOLDic.Head() };

    while (current) {
        if (name == current->name) return current;
        else current = current->next;
    }

    return current;
}
static void InSert_SL(const string name, int hp, int mp, int speed, int range, const string position)
{
    // 연결리스트 클래스쪽을 봐주세요.
    LOLDic.append(name, hp, mp, speed, range, position);
}
static int Delete_SL(const string name)
{
    Node* current{ LOLDic.Head() };
    Node* prdv{ nullptr };
    
    while (current) {
        if (name == current->name) {
            Node* nextNode = current->next; // 다음 노드를 미리 저장
            if (prdv == nullptr)            // 헤드가 타겟일 때
                LOLDic.changehead(nextNode);
            else
                prdv->next = nextNode;
            delete current;
            current = nextNode;
            LOLDic.Size--;
            return 0;
        }
        prdv = current;
        current = current->next;
    }
    return 1;
}
static int DeleteAll_SL(const string pos)
{
    Node* current{ LOLDic.Head() };
    Node* prdv{ nullptr };

    while (current != nullptr) {
        if (pos == current->position) {
            Node* nextNode = current->next; // 다음 노드를 미리 저장
            if (prdv == nullptr)            // 헤드가 타겟일 때
                LOLDic.changehead(nextNode);
            else
                prdv->next = nextNode;
            delete current;
            current = nextNode;
            LOLDic.Size--;
        }
        else {
            prdv = current;
            current = current->next;
        }
    }
    return 0;
}
static void PrintAll_SL()
{
    Node* current{ LOLDic.Head() };
    int Num{ 1 };

    while (current) {
        cout << setw(3) << Num++ << " ";
        current->display();
        current = current->next;
    }
}
static void FindMaxHp_SL()
{
    Node* current{ LOLDic.Head() };
    int MaxHp{ numeric_limits<int>::min() };

    while (current) {
        if (MaxHp <= current->hp) {
            MaxHp = current->hp;
        }
        current = current->next;
    }

    cout << "MaxHp : " << MaxHp << endl;
    current = LOLDic.Head();
    while (current) {
        if (MaxHp == current->hp) {
            current->display();
        }
        current = current->next;
    }

    return;
}
static void SortByHp_SL(int start, int end)
{
    // 연결리스트 클래스쪽을 봐주세요.
    LOLDic.mergeSort(LOLDic.Head());
}

constexpr int MAX_LINE_LENGTH{ 256 }; // 적절한 최대 라인 길이 정의
constexpr int MAX_FIELD_LENGTH{ 64 }; // 각 필드의 최대 길이 정의
constexpr int MAX_FIELDS{ 6 };

void InSertTestData() {
    ifstream in("testdata.txt");
    if (!in.is_open()) {
        cerr << "Error opening testdata.txt" << endl;
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (in.getline(line, MAX_LINE_LENGTH)) { // getline은 istream의 멤버 함수
        char* token;
        char* context{ nullptr };
        char fields[MAX_FIELDS][MAX_FIELD_LENGTH]{};
        int index{};

        token = strtok_s(line, "\t\t", &context); // 첫 번째 토큰 분리
        while (token != nullptr && index < MAX_FIELDS - 1) {
            strncpy_s(fields[index++], MAX_FIELD_LENGTH, token, _TRUNCATE); // 토큰 복사
            token = strtok_s(nullptr, "\t\t", &context); // 다음 토큰 분리
        }

        if (index == MAX_FIELDS - 1 && token != nullptr) { // 마지막 데이터 처리
            strncpy_s(fields[index], MAX_FIELD_LENGTH, token, _TRUNCATE);
            index++;
        }

        if (index == MAX_FIELDS) { // 데이터가 올바르게 6개 저장되었는지 확인
            char name[MAX_FIELD_LENGTH], position[MAX_FIELD_LENGTH];
            int hp, mp, speed, range;

            strncpy_s(name, MAX_FIELD_LENGTH, fields[0], _TRUNCATE);
            strncpy_s(position, MAX_FIELD_LENGTH, fields[5], _TRUNCATE);
            hp = atoi(fields[1]); mp = atoi(fields[2]); speed = atoi(fields[3]); range = atoi(fields[4]);
            
            LOLDic.append(name, hp, mp, speed, range, position);
        }
    }

    in.close();
}
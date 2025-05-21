#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost"; // 필요에 따라 변경
const char* user = "root";      // 필요에 따라 변경
const char* pw = "20212020";    // 필요에 따라 변경
const char* db = "project";     // 데이터베이스 이름

using namespace std;

vector<string> dropCommands; // DROP 명령어를 저장할 벡터

// MySQL 연결 및 설정
MYSQL* connectDatabase() {
    MYSQL* connection = mysql_init(NULL);
    if (connection == NULL) {
        printf("mysql_init() error!");
        return NULL;
    }
    connection = mysql_real_connect(connection, host, user, pw, NULL, 3306, NULL, 0);
    if (connection == NULL) {
        printf("%d ERROR : %s\n", mysql_errno(connection), mysql_error(connection));
    }
    return connection;
}

// CRUD 파일 처리 함수
void processCrudFile(MYSQL* connection, const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        printf("Cannot open CRUD file.\n");
        return;
    }
    string line;
    bool isDropSection = false;

    while (getline(file, line)) {
        if (line == "$$$") {
            isDropSection = true;
            continue;
        }

        if (isDropSection) {
            if (!line.empty()) {
                dropCommands.push_back(line);
            }
        }
        else {
            if (!line.empty()) {
                if (mysql_query(connection, line.c_str())) {
                    printf("Query Error: %s\n", mysql_error(connection));
                    printf("Query: %s\n", line.c_str()); // 오류가 발생한 쿼리 출력
                }
            }
        }
    }
    file.close();
}

// 쿼리 수행 함수
void executeQuery(MYSQL* connection, const string& query, bool printHeader) {
    if (mysql_query(connection, query.c_str())) {
        cerr << "Query Error: " << mysql_error(connection) << '\n';
        return;
    }
    MYSQL_RES* res = mysql_store_result(connection);
    if (res) {
        MYSQL_ROW row;
        unsigned int num_fields = mysql_num_fields(res);
        MYSQL_FIELD* fields = mysql_fetch_fields(res);

        if (printHeader) {
            // 컬럼 헤더 출력
            for (unsigned int i = 0; i < num_fields; i++) {
                cout << left << setw(32) << ("[" + string(fields[i].name) + "]");
            }
            cout << '\n';
        }

        // 데이터 출력
        while ((row = mysql_fetch_row(res))) {
            for (unsigned int i = 0; i < num_fields; i++) {
                cout << left << setw(32) << (row[i] ? row[i] : "NULL");
            }
            cout << '\n';
        }
        mysql_free_result(res);
    }
}

// TYPE 1 쿼리 함수
void type1Queries(MYSQL* connection) {
    string query;
    query = "SELECT Address, District FROM Properties WHERE ForSale=1 AND District='Mapo';";
    executeQuery(connection, query, true);

    int choice;
    while (true) {
        cout << "\n------- Subtypes in TYPE 1 -------\n";
        cout << "       1. TYPE 1-1\n\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << '\n';
            query = "SELECT PropertyID, District, Price FROM Properties WHERE ForSale=1 AND District='Mapo' AND Price BETWEEN 1000000000 AND 1500000000;";
            executeQuery(connection, query, true);
            return; // 초기 메뉴로 돌아가기
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

// TYPE 2 쿼리 함수
void type2Queries(MYSQL* connection) {
    string query;
    query = "SELECT Address, District FROM Properties WHERE ForSale=1 AND District IN ('Dongdaemun', 'Jungnang', 'Mapo', 'Seodaemun', 'Eunpyeong', 'Guro', 'Geumcheon', 'Yeongdeungpo', 'Nowon', 'Dobong', 'Yongsan', 'Jongno', 'Jung', 'Gangdong', 'Songpa', 'Gangseo', 'Yangcheon', 'Gangnam', 'Seocho');";
    executeQuery(connection, query, true);

    int choice;
    while (true) {
        cout << "\n------- Subtypes in TYPE 2 -------\n";
        cout << "       1. TYPE 2-1\n\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << '\n';
            query = "SELECT p.Address, r.BedroomCount, r.BathroomCount FROM Properties p JOIN Rooms r ON p.PropertyID = r.PropertyID WHERE p.ForSale=1 AND p.District IN ('Dongdaemun', 'Jungnang', 'Mapo', 'Seodaemun', 'Eunpyeong', 'Guro', 'Geumcheon', 'Yeongdeungpo', 'Nowon', 'Dobong', 'Yongsan', 'Jongno', 'Jung', 'Gangdong', 'Songpa', 'Gangseo', 'Yangcheon', 'Gangnam', 'Seocho') AND r.BedroomCount >= 4 AND r.BathroomCount >= 2;";
            executeQuery(connection, query, true);
            return; // 초기 메뉴로 돌아가기
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

// TYPE 3 쿼리 함수
void type3Queries(MYSQL* connection) {
    string query;
    // 가장 많은 판매 원화가치의 에이전트를 찾는 쿼리
    query = "SELECT a.AgentName, SUM(s.SalePrice) AS TotalSales "
        "FROM Agents a JOIN Sales s ON a.AgentID = s.AgentID "
        "WHERE YEAR(s.SaleDate) = 2022 "
        "GROUP BY a.AgentID "
        "ORDER BY TotalSales DESC "
        "LIMIT 1;";
    executeQuery(connection, query, true);

    int choice;
    while (true) {
        cout << "\n------- Subtypes in TYPE 3 -------\n";
        cout << "       1. TYPE 3-1\n";
        cout << "       2. TYPE 3-2\n\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "\n------- TYPE 3-1 --------\n";
            int k;
            cout << "\n** Then find the top k agents in the year 2023 by total won value. **\n";
            cout << " Which K? : ";
            cin >> k;
            cout << '\n';
            // 상위 k명의 에이전트를 찾는 쿼리
            query = "SELECT a.AgentName, SUM(s.SalePrice) AS TotalSales "
                "FROM Agents a JOIN Sales s ON a.AgentID = s.AgentID "
                "WHERE YEAR(s.SaleDate) = 2023 "
                "GROUP BY a.AgentID "
                "ORDER BY TotalSales DESC "
                "LIMIT " + to_string(k) + ";";
            executeQuery(connection, query, true);
            return; // 초기 메뉴로 돌아가기
        }
        case 2: {
            cout << '\n';
            // 하위 10%의 에이전트를 찾기 위해 전체 에이전트 수의 10% 계산
            string countQuery = "SELECT ROUND(COUNT(DISTINCT a.AgentID) * 0.1) AS Bottom10PercentCount "
                "FROM Agents a JOIN Sales s ON a.AgentID = s.AgentID "
                "WHERE YEAR(s.SaleDate) = 2021;";
            if (mysql_query(connection, countQuery.c_str())) {
                printf("Query Error: %s\n", mysql_error(connection));
                return;
            }
            MYSQL_RES* res = mysql_store_result(connection);
            if (!res) {
                printf("Result Error: %s\n", mysql_error(connection));
                return;
            }
            MYSQL_ROW row = mysql_fetch_row(res);
            if (!row) {
                printf("Fetch Error: %s\n", mysql_error(connection));
                mysql_free_result(res);
                return;
            }
            int bottom10PercentCount = stoi(row[0] ? row[0] : "0");
            mysql_free_result(res);

            // 하위 10%의 에이전트를 찾는 쿼리
            query = "SELECT AgentName, TotalSales "
                "FROM (SELECT a.AgentName, SUM(s.SalePrice) AS TotalSales "
                "FROM Agents a JOIN Sales s ON a.AgentID = s.AgentID "
                "WHERE YEAR(s.SaleDate) = 2021 "
                "GROUP BY a.AgentID "
                "ORDER BY TotalSales ASC) AS SortedAgents "
                "LIMIT " + to_string(bottom10PercentCount) + ";";
            executeQuery(connection, query, true);
            return; // 초기 메뉴로 돌아가기
        }
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

// TYPE 4 쿼리 함수
void type4Queries(MYSQL* connection) {
    string query;
    // 2022년 평균 판매 가격과 평균 시장에 나와 있는 시간 계산
    query = "SELECT a.AgentName, AVG(s.SalePrice) AS AvgSalePrice, AVG(DATEDIFF(s.SaleDate, p.ListedDate)) AS AvgMarketTime "
        "FROM Agents a JOIN Sales s ON a.AgentID = s.AgentID "
        "JOIN Properties p ON s.PropertyID = p.PropertyID "
        "WHERE YEAR(s.SaleDate) = 2022 "
        "GROUP BY a.AgentID;";
    executeQuery(connection, query, true);

    int choice;
    while (true) {
        cout << "\n------- Subtypes in TYPE 4 -------\n";
        cout << "       1. TYPE 4-1\n";
        cout << "       2. TYPE 4-2\n\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << '\n';
            // 2023년 최대 판매 가격 계산
            query = "SELECT a.AgentName, MAX(s.SalePrice) AS MaxSalePrice "
                "FROM Agents a JOIN Sales s ON a.AgentID = s.AgentID "
                "WHERE YEAR(s.SaleDate) = 2023 "
                "GROUP BY a.AgentID;";
            executeQuery(connection, query, true);
            return; // 초기 메뉴로 돌아가기
        case 2:
            cout << '\n';
            // 시장에 나와 있는 가장 긴 시간 계산
            query = "SELECT a.AgentName, MAX(DATEDIFF(s.SaleDate, p.ListedDate)) AS LongestMarketTime "
                "FROM Agents a JOIN Sales s ON a.AgentID = s.AgentID "
                "JOIN Properties p ON s.PropertyID = p.PropertyID "
                "GROUP BY a.AgentID;";
            executeQuery(connection, query, true);
            return; // 초기 메뉴로 돌아가기
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

// TYPE 5 쿼리 함수
void type5Queries(MYSQL* connection) {
    string query;

    // 가장 비싼 studio 부동산의 사진 찾기
    query = "SELECT ph.PhotoID, p.PropertyID, p.PropertyType, ph.PhotoType "
        "FROM Photos ph "
        "JOIN Properties p ON ph.PropertyID = p.PropertyID "
        "WHERE p.PropertyType = 'studio' "
        "ORDER BY p.Price DESC "
        "LIMIT 1;";
    executeQuery(connection, query, true);

    // 가장 비싼 one-bedroom 부동산의 사진 찾기
    query = "SELECT ph.PhotoID, p.PropertyID, p.PropertyType, ph.PhotoType "
        "FROM Photos ph "
        "JOIN Properties p ON ph.PropertyID = p.PropertyID "
        "WHERE p.PropertyType = 'one-bedroom' "
        "ORDER BY p.Price DESC "
        "LIMIT 1;";
    executeQuery(connection, query, false);

    // 가장 비싼 multi-bedroom 부동산의 사진 찾기
    query = "SELECT ph.PhotoID, p.PropertyID, p.PropertyType, ph.PhotoType "
        "FROM Photos ph "
        "JOIN Properties p ON ph.PropertyID = p.PropertyID "
        "WHERE p.PropertyType = 'multi-bedroom' "
        "ORDER BY p.Price DESC "
        "LIMIT 1;";
    executeQuery(connection, query, false);

    // 가장 비싼 detached 부동산의 사진 찾기
    query = "SELECT ph.PhotoID, p.PropertyID, p.PropertyType, ph.PhotoType "
        "FROM Photos ph "
        "JOIN Properties p ON ph.PropertyID = p.PropertyID "
        "WHERE p.PropertyType = 'detached' "
        "ORDER BY p.Price DESC "
        "LIMIT 1;";
    executeQuery(connection, query, false);
}

// TYPE 6 쿼리 함수
void type6Queries(MYSQL* connection) {
    cout << "\nTYPE 6 - Record a sale:\n\n";
    string saleID, propertyID, salePrice, saleDate, buyerID, agentID;
    cout << "Enter SaleID: ";
    cin >> saleID;
    cout << "Enter PropertyID: ";
    cin >> propertyID;
    cout << "Enter SalePrice: ";
    cin >> salePrice;
    cout << "Enter SaleDate (YYYY-MM-DD): ";
    cin >> saleDate;
    cout << "Enter BuyerID: ";
    cin >> buyerID;
    cout << "Enter AgentID: ";
    cin >> agentID;

    ostringstream query;
    query << "INSERT INTO Sales (SaleID, PropertyID, SalePrice, SaleDate, BuyerID, AgentID) "
        << "VALUES ('" << saleID << "', '" << propertyID << "', '" << salePrice << "', '" << saleDate << "', '" << buyerID << "', '" << agentID << "');";

    executeQuery(connection, query.str(), true);
}

// TYPE 7 쿼리 함수
void type7Queries(MYSQL* connection) {
    cout << "\nTYPE 7 - Add a new agent:\n\n";
    string agentID, agentName, phoneNumber;
    cout << "Enter AgentID: ";
    cin >> agentID;
    cout << "Enter AgentName: ";
    cin.ignore();  // 버퍼 비우기
    getline(cin, agentName);
    cout << "Enter PhoneNumber: ";
    cin >> phoneNumber;
    string query = "INSERT INTO Agents (AgentID, AgentName, PhoneNumber) VALUES ('" + agentID + "', '" + agentName + "', '" + phoneNumber + "');";
    executeQuery(connection, query, true);
}

// 메인 메뉴 제공 함수
void queryMenu(MYSQL* connection) {
    int choice;
    while (true) {
        cout << "\n---------- SELECT QUERY TYPES ----------\n\n";
        cout << "           1. TYPE 1\n";
        cout << "           2. TYPE 2\n";
        cout << "           3. TYPE 3\n";
        cout << "           4. TYPE 4\n";
        cout << "           5. TYPE 5\n";
        cout << "           6. TYPE 6\n";
        cout << "           7. TYPE 7\n";
        cout << "           0. QUIT\n\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << '\n';
            type1Queries(connection);
            break;
        case 2:
            cout << '\n';
            type2Queries(connection);
            break;
        case 3:
            cout << '\n';
            type3Queries(connection);
            break;
        case 4:
            cout << '\n';
            type4Queries(connection);
            break;
        case 5:
            cout << '\n';
            type5Queries(connection);
            break;
        case 6:
            cout << '\n';
            type6Queries(connection);
            break;
        case 7:
            cout << '\n';
            type7Queries(connection);
            break;
        case 0:
            // DROP 명령어 실행
            for (const string& dropCommand : dropCommands) {
                if (mysql_query(connection, dropCommand.c_str())) {
                    printf("Query Error: %s\n", mysql_error(connection));
                    printf("Query: %s\n", dropCommand.c_str()); // 오류가 발생한 쿼리 출력
                }
            }
            return;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

// 메인 함수
int main(void) {
    MYSQL* connection = connectDatabase();
    if (connection == NULL) {
        printf("Database connection failed.\n");
        return 1;
    }

    processCrudFile(connection, "CRUD.txt");

    if (mysql_select_db(connection, db)) {
        printf("%d ERROR : %s\n", mysql_errno(connection), mysql_error(connection));
        return 1;
    }

    queryMenu(connection);

    mysql_close(connection);
    return 0;
}

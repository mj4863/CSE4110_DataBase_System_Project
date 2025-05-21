#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost"; // �ʿ信 ���� ����
const char* user = "root";      // �ʿ信 ���� ����
const char* pw = "20212020";    // �ʿ信 ���� ����
const char* db = "project";     // �����ͺ��̽� �̸�

using namespace std;

vector<string> dropCommands; // DROP ��ɾ ������ ����

// MySQL ���� �� ����
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

// CRUD ���� ó�� �Լ�
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
                    printf("Query: %s\n", line.c_str()); // ������ �߻��� ���� ���
                }
            }
        }
    }
    file.close();
}

// ���� ���� �Լ�
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
            // �÷� ��� ���
            for (unsigned int i = 0; i < num_fields; i++) {
                cout << left << setw(32) << ("[" + string(fields[i].name) + "]");
            }
            cout << '\n';
        }

        // ������ ���
        while ((row = mysql_fetch_row(res))) {
            for (unsigned int i = 0; i < num_fields; i++) {
                cout << left << setw(32) << (row[i] ? row[i] : "NULL");
            }
            cout << '\n';
        }
        mysql_free_result(res);
    }
}

// TYPE 1 ���� �Լ�
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
            return; // �ʱ� �޴��� ���ư���
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

// TYPE 2 ���� �Լ�
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
            return; // �ʱ� �޴��� ���ư���
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

// TYPE 3 ���� �Լ�
void type3Queries(MYSQL* connection) {
    string query;
    // ���� ���� �Ǹ� ��ȭ��ġ�� ������Ʈ�� ã�� ����
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
            // ���� k���� ������Ʈ�� ã�� ����
            query = "SELECT a.AgentName, SUM(s.SalePrice) AS TotalSales "
                "FROM Agents a JOIN Sales s ON a.AgentID = s.AgentID "
                "WHERE YEAR(s.SaleDate) = 2023 "
                "GROUP BY a.AgentID "
                "ORDER BY TotalSales DESC "
                "LIMIT " + to_string(k) + ";";
            executeQuery(connection, query, true);
            return; // �ʱ� �޴��� ���ư���
        }
        case 2: {
            cout << '\n';
            // ���� 10%�� ������Ʈ�� ã�� ���� ��ü ������Ʈ ���� 10% ���
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

            // ���� 10%�� ������Ʈ�� ã�� ����
            query = "SELECT AgentName, TotalSales "
                "FROM (SELECT a.AgentName, SUM(s.SalePrice) AS TotalSales "
                "FROM Agents a JOIN Sales s ON a.AgentID = s.AgentID "
                "WHERE YEAR(s.SaleDate) = 2021 "
                "GROUP BY a.AgentID "
                "ORDER BY TotalSales ASC) AS SortedAgents "
                "LIMIT " + to_string(bottom10PercentCount) + ";";
            executeQuery(connection, query, true);
            return; // �ʱ� �޴��� ���ư���
        }
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

// TYPE 4 ���� �Լ�
void type4Queries(MYSQL* connection) {
    string query;
    // 2022�� ��� �Ǹ� ���ݰ� ��� ���忡 ���� �ִ� �ð� ���
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
            // 2023�� �ִ� �Ǹ� ���� ���
            query = "SELECT a.AgentName, MAX(s.SalePrice) AS MaxSalePrice "
                "FROM Agents a JOIN Sales s ON a.AgentID = s.AgentID "
                "WHERE YEAR(s.SaleDate) = 2023 "
                "GROUP BY a.AgentID;";
            executeQuery(connection, query, true);
            return; // �ʱ� �޴��� ���ư���
        case 2:
            cout << '\n';
            // ���忡 ���� �ִ� ���� �� �ð� ���
            query = "SELECT a.AgentName, MAX(DATEDIFF(s.SaleDate, p.ListedDate)) AS LongestMarketTime "
                "FROM Agents a JOIN Sales s ON a.AgentID = s.AgentID "
                "JOIN Properties p ON s.PropertyID = p.PropertyID "
                "GROUP BY a.AgentID;";
            executeQuery(connection, query, true);
            return; // �ʱ� �޴��� ���ư���
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

// TYPE 5 ���� �Լ�
void type5Queries(MYSQL* connection) {
    string query;

    // ���� ��� studio �ε����� ���� ã��
    query = "SELECT ph.PhotoID, p.PropertyID, p.PropertyType, ph.PhotoType "
        "FROM Photos ph "
        "JOIN Properties p ON ph.PropertyID = p.PropertyID "
        "WHERE p.PropertyType = 'studio' "
        "ORDER BY p.Price DESC "
        "LIMIT 1;";
    executeQuery(connection, query, true);

    // ���� ��� one-bedroom �ε����� ���� ã��
    query = "SELECT ph.PhotoID, p.PropertyID, p.PropertyType, ph.PhotoType "
        "FROM Photos ph "
        "JOIN Properties p ON ph.PropertyID = p.PropertyID "
        "WHERE p.PropertyType = 'one-bedroom' "
        "ORDER BY p.Price DESC "
        "LIMIT 1;";
    executeQuery(connection, query, false);

    // ���� ��� multi-bedroom �ε����� ���� ã��
    query = "SELECT ph.PhotoID, p.PropertyID, p.PropertyType, ph.PhotoType "
        "FROM Photos ph "
        "JOIN Properties p ON ph.PropertyID = p.PropertyID "
        "WHERE p.PropertyType = 'multi-bedroom' "
        "ORDER BY p.Price DESC "
        "LIMIT 1;";
    executeQuery(connection, query, false);

    // ���� ��� detached �ε����� ���� ã��
    query = "SELECT ph.PhotoID, p.PropertyID, p.PropertyType, ph.PhotoType "
        "FROM Photos ph "
        "JOIN Properties p ON ph.PropertyID = p.PropertyID "
        "WHERE p.PropertyType = 'detached' "
        "ORDER BY p.Price DESC "
        "LIMIT 1;";
    executeQuery(connection, query, false);
}

// TYPE 6 ���� �Լ�
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

// TYPE 7 ���� �Լ�
void type7Queries(MYSQL* connection) {
    cout << "\nTYPE 7 - Add a new agent:\n\n";
    string agentID, agentName, phoneNumber;
    cout << "Enter AgentID: ";
    cin >> agentID;
    cout << "Enter AgentName: ";
    cin.ignore();  // ���� ����
    getline(cin, agentName);
    cout << "Enter PhoneNumber: ";
    cin >> phoneNumber;
    string query = "INSERT INTO Agents (AgentID, AgentName, PhoneNumber) VALUES ('" + agentID + "', '" + agentName + "', '" + phoneNumber + "');";
    executeQuery(connection, query, true);
}

// ���� �޴� ���� �Լ�
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
            // DROP ��ɾ� ����
            for (const string& dropCommand : dropCommands) {
                if (mysql_query(connection, dropCommand.c_str())) {
                    printf("Query Error: %s\n", mysql_error(connection));
                    printf("Query: %s\n", dropCommand.c_str()); // ������ �߻��� ���� ���
                }
            }
            return;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}

// ���� �Լ�
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

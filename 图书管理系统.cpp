#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

using namespace std;

// 抽象基类：统一“可显示”的接口
class Displayable {
public:
    virtual ~Displayable() = default;
    virtual void display() const = 0;
};

// 图书类
class Book : public Displayable {
private:
    int bookId;           // 图书编号
    string title;         // 书名
    string author;        // 作者
    string isbn;          // ISBN号
    bool isBorrowed;      // 是否被借出
    int borrowerId;       // 借阅者ID

public:
    // 构造函数
    Book(int id, const string& t, const string& a, const string& i)
        : bookId(id), title(t), author(a), isbn(i), isBorrowed(false), borrowerId(-1) {}

    // getter方法
    int getId() const { return bookId; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getIsbn() const { return isbn; }
    bool getBorrowed() const { return isBorrowed; }
    int getBorrowerId() const { return borrowerId; }

    // setter方法
    void setBorrowed(bool borrowed) { isBorrowed = borrowed; }
    void setBorrowerId(int id) { borrowerId = id; }

    // 显示图书信息
    void display() const override {
        cout << "[" << bookId << "] 《" << title << "》 - " << author
             << " (ISBN:" << isbn << ") ";
        if (isBorrowed) {
            cout << "[已借出]";
        } else {
            cout << "[可借]";
        }
        cout << endl;
    }
};

// 借阅记录结构体
struct BorrowRecord {
    int bookId;
    string bookTitle;
    string borrowDate;
    string returnDate;

    BorrowRecord(int bid, const string& title, const string& bDate)
        : bookId(bid), bookTitle(title), borrowDate(bDate), returnDate("未归还") {}
};

// 用户类
class User : public Displayable {
private:
    int userId;
    string name;
    vector<int> borrowedBooks;
    vector<BorrowRecord> history;

public:
    // 构造函数
    User(int id, const string& n) : userId(id), name(n) {}

    // getter方法
    int getId() const { return userId; }
    string getName() const { return name; }
    vector<int> getBorrowedBooks() const { return borrowedBooks; }

    // 借书
    bool borrowBook(int bookId, const string& bookTitle, const string& date) {
        // 检查是否已借阅超过5本
        if (borrowedBooks.size() >= 5) {
            cout << "借阅失败：每人最多借阅5本书" << endl;
            return false;
        }

        borrowedBooks.push_back(bookId);
        history.push_back(BorrowRecord(bookId, bookTitle, date));
        return true;
    }

    // 还书
    bool returnBook(int bookId, const string& returnDate) {
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), bookId);
        if (it == borrowedBooks.end()) {
            return false;
        }

        borrowedBooks.erase(it);

        // 更新历史记录
        for (auto& record : history) {
            if (record.bookId == bookId && record.returnDate == "未归还") {
                record.returnDate = returnDate;
                break;
            }
        }
        return true;
    }

    // 显示用户信息
    void display() const override {
        cout << "[" << userId << "] " << name << " (当前借阅 " << borrowedBooks.size() << " 本)" << endl;
    }

    // 显示当前借阅
    void showBorrowed() const {
        if (borrowedBooks.empty()) {
            cout << "  暂无借阅图书" << endl;
            return;
        }

        cout << "  当前借阅的图书ID：";
        for (int id : borrowedBooks) {
            cout << id << " ";
        }
        cout << endl;
    }
};

// 图书馆管理系统类
class Library {
private:
    string name;
    map<int, Book> books;        // 图书映射
    map<int, User> users;        // 用户映射
    int nextBookId;
    int nextUserId;

    // 读取整数
    bool readInt(const string& prompt, int& value) {
        cout << prompt;
        if (!(cin >> value)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入无效，请输入数字！" << endl;
            return false;
        }
        return true;
    }

    // 读取一整行字符串
    string readLine(const string& prompt) {
        cout << prompt;
        string s;
        getline(cin >> ws, s);
        return s;
    }

public:
    // 构造函数
    Library(const string& n = "智慧图书馆")
        : name(n), nextBookId(1), nextUserId(1) {}

    // ---------- 图书管理 ----------
    void addBook() {
        string title = readLine("请输入书名：");
        string author = readLine("请输入作者：");
        string isbn = readLine("请输入ISBN：");

        books.emplace(nextBookId, Book(nextBookId, title, author, isbn));
        cout << "成功添加图书，编号为：" << nextBookId << endl;
        nextBookId++;
    }

    void removeBook() {
        int bookId;
        if (!readInt("请输入要删除的图书编号：", bookId)) return;

        auto it = books.find(bookId);
        if (it == books.end()) {
            cout << "错误：图书编号不存在" << endl;
            return;
        }

        if (it->second.getBorrowed()) {
            cout << "错误：该图书已被借出，无法删除" << endl;
            return;
        }

        books.erase(it);
        cout << "成功删除图书" << endl;
    }

    void searchBook() {
        cout << "=== 图书查询 ===" << endl;
        cout << "1. 按编号查询" << endl;
        cout << "2. 按书名查询" << endl;
        cout << "3. 列出所有图书" << endl;

        int choice;
        if (!readInt("请选择：", choice)) return;

        if (choice == 1) {
            int bookId;
            if (!readInt("请输入图书编号：", bookId)) return;

            auto it = books.find(bookId);
            if (it != books.end()) {
                it->second.display();
            } else {
                cout << "未找到该图书" << endl;
            }
        }
        else if (choice == 2) {
            string title = readLine("请输入书名：");

            bool found = false;
            for (const auto& pair : books) {
                if (pair.second.getTitle().find(title) != string::npos) {
                    pair.second.display();
                    found = true;
                }
            }
            if (!found) {
                cout << "未找到相关图书" << endl;
            }
        }
        else if (choice == 3) {
            showAllBooks();
        }
        else {
            cout << "无效选择" << endl;
        }
    }

    void showAllBooks() const {
        if (books.empty()) {
            cout << "图书馆暂无藏书" << endl;
            return;
        }

        cout << "\n=== 全部藏书 ===" << endl;
        for (const auto& pair : books) {
            pair.second.display();
        }
        cout << endl;
    }

    // ---------- 用户管理 ----------
    void addUser() {
        string name = readLine("请输入用户名：");

        users.emplace(nextUserId, User(nextUserId, name));
        cout << "成功添加用户，编号为：" << nextUserId << endl;
        nextUserId++;
    }

    void removeUser() {
        int userId;
        if (!readInt("请输入要删除的用户编号：", userId)) return;

        auto it = users.find(userId);
        if (it == users.end()) {
            cout << "错误：用户编号不存在" << endl;
            return;
        }

        if (!it->second.getBorrowedBooks().empty()) {
            cout << "错误：该用户还有未归还的图书，无法删除" << endl;
            return;
        }

        users.erase(it);
        cout << "成功删除用户" << endl;
    }

    void showAllUsers() const {
        if (users.empty()) {
            cout << "暂无注册用户" << endl;
            return;
        }

        cout << "\n=== 全部用户 ===" << endl;
        for (const auto& pair : users) {
            pair.second.display();
        }
        cout << endl;
    }

    // ---------- 借阅管理 ----------
    void borrowBook() {
        int userId, bookId;
        string date;

        if (!readInt("请输入用户编号：", userId)) return;
        if (!readInt("请输入图书编号：", bookId)) return;
        date = readLine("请输入借书日期（如：2024-01-15）：");

        auto userIt = users.find(userId);
        if (userIt == users.end()) {
            cout << "错误：用户不存在" << endl;
            return;
        }

        auto bookIt = books.find(bookId);
        if (bookIt == books.end()) {
            cout << "错误：图书不存在" << endl;
            return;
        }

        if (bookIt->second.getBorrowed()) {
            cout << "错误：该书已被借出" << endl;
            return;
        }

        if (userIt->second.borrowBook(bookId, bookIt->second.getTitle(), date)) {
            bookIt->second.setBorrowed(true);
            bookIt->second.setBorrowerId(userId);
            cout << "借阅成功！" << endl;
        }
    }

    void returnBook() {
        int userId, bookId;
        string date;

        if (!readInt("请输入用户编号：", userId)) return;
        if (!readInt("请输入图书编号：", bookId)) return;
        date = readLine("请输入还书日期（如：2024-02-20）：");

        auto userIt = users.find(userId);
        if (userIt == users.end()) {
            cout << "错误：用户不存在" << endl;
            return;
        }

        auto bookIt = books.find(bookId);
        if (bookIt == books.end()) {
            cout << "错误：图书不存在" << endl;
            return;
        }

        if (bookIt->second.getBorrowerId() != userId) {
            cout << "错误：该图书不是由此用户借阅" << endl;
            return;
        }

        if (userIt->second.returnBook(bookId, date)) {
            bookIt->second.setBorrowed(false);
            bookIt->second.setBorrowerId(-1);
            cout << "归还成功！" << endl;
        } else {
            cout << "归还失败" << endl;
        }
    }

    void showUserBorrowed() {
        int userId;
        if (!readInt("请输入用户编号：", userId)) return;

        auto it = users.find(userId);
        if (it == users.end()) {
            cout << "错误：用户不存在" << endl;
            return;
        }

        it->second.display();
        it->second.showBorrowed();
    }

    void showMenu() {
        cout << "\n========================================" << endl;
        cout << "        欢迎使用 " << name << endl;
        cout << "========================================" << endl;
        cout << "1. 添加图书" << endl;
        cout << "2. 删除图书" << endl;
        cout << "3. 查询图书" << endl;
        cout << "4. 显示所有图书" << endl;
        cout << "5. 添加用户" << endl;
        cout << "6. 删除用户" << endl;
        cout << "7. 显示所有用户" << endl;
        cout << "8. 借阅图书" << endl;
        cout << "9. 归还图书" << endl;
        cout << "10. 查看用户借阅" << endl;
        cout << "0. 退出系统" << endl;
        cout << "========================================" << endl;
        cout << "请选择操作：";
    }

    void run() {
        int choice;
        do {
            showMenu();

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "输入无效，请输入数字菜单选项！" << endl;
                continue;
            }

            switch (choice) {
                case 1: addBook(); break;
                case 2: removeBook(); break;
                case 3: searchBook(); break;
                case 4: showAllBooks(); break;
                case 5: addUser(); break;
                case 6: removeUser(); break;
                case 7: showAllUsers(); break;
                case 8: borrowBook(); break;
                case 9: returnBook(); break;
                case 10: showUserBorrowed(); break;
                case 0:
                    cout << "感谢使用，再见！" << endl;
                    break;
                default:
                    cout << "无效选择，请重新输入！" << endl;
            }
        } while (choice != 0);
    }
};

// 主函数
int main() {
    Library lib("智慧图书馆");
    lib.run();
    return 0;
}

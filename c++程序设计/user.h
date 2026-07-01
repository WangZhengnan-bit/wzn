#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <memory>

// 前向声明
class Order;

// ==================== 用户基类 ====================
class User {
protected:
    int         m_id;
    std::string m_username;
    std::string m_password;

public:
    User(int id, const std::string& username, const std::string& password);
    virtual ~User() = default;

    // ---- 纯虚函数：在派生类中必须重写 ----
    virtual void        display()  const = 0;
    virtual std::string getRole()  const = 0;

    // ---- 公共访问器 ----
    int                getId()       const { return m_id; }
    const std::string& getUsername() const { return m_username; }

    // ---- 密码校验 ----
    bool checkPassword(const std::string& pwd) const { return m_password == pwd; }
};

// ==================== 管理员类 ====================
class Admin : public User {
public:
    Admin(int id, const std::string& username, const std::string& password);

    void        display() const override;
    std::string getRole() const override;
};

// ==================== 顾客类 ====================
class Customer : public User {
private:
    std::vector<std::shared_ptr<Order>> m_orderHistory; // 订单历史

public:
    Customer(int id, const std::string& username, const std::string& password);

    void        display() const override;
    std::string getRole() const override;

    // ---- 顾客特有操作 ----
    void addOrder(std::shared_ptr<Order> order);
    void viewOrderHistory() const;
    const std::vector<std::shared_ptr<Order>>& getOrders() const;
};

#endif // USER_H

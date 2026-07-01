#include "user.h"
#include "order.h"
#include <iostream>

// ==================== User 基类 ====================
User::User(int id, const std::string& username, const std::string& password)
    : m_id(id), m_username(username), m_password(password) {}

// ==================== Admin ====================
Admin::Admin(int id, const std::string& username, const std::string& password)
    : User(id, username, password) {}

void Admin::display() const {
    std::cout << "┌─────────── 管理员信息 ───────────┐\n";
    std::cout << "│ ID:    " << m_id << "\n";
    std::cout << "│ 用户名: " << m_username << "\n";
    std::cout << "│ 角色:   " << getRole() << "\n";
    std::cout << "└──────────────────────────────────┘\n";
}

std::string Admin::getRole() const {
    return "管理员";
}

// ==================== Customer ====================
Customer::Customer(int id, const std::string& username, const std::string& password)
    : User(id, username, password) {}

void Customer::display() const {
    std::cout << "┌─────────── 顾客信息 ────────────┐\n";
    std::cout << "│ ID:    " << m_id << "\n";
    std::cout << "│ 用户名: " << m_username << "\n";
    std::cout << "│ 角色:   " << getRole() << "\n";
    std::cout << "│ 历史订单: " << m_orderHistory.size() << " 笔\n";
    std::cout << "└──────────────────────────────────┘\n";
}

std::string Customer::getRole() const {
    return "顾客";
}

void Customer::addOrder(std::shared_ptr<Order> order) {
    m_orderHistory.push_back(std::move(order));
}

void Customer::viewOrderHistory() const {
    if (m_orderHistory.empty()) {
        std::cout << "  暂无订单记录。\n";
        return;
    }
    for (const auto& order : m_orderHistory) {
        order->display();
    }
}

const std::vector<std::shared_ptr<Order>>& Customer::getOrders() const {
    return m_orderHistory;
}

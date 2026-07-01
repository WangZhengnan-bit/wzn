#ifndef STORE_H
#define STORE_H

#include <vector>
#include <memory>
#include <string>
#include "product.h"
#include "user.h"
#include "order.h"
#include "cart.h"

// ==================== 商店（系统核心） ====================
class Store {
private:
    std::vector<std::shared_ptr<Product>> m_products;
    std::vector<std::shared_ptr<User>>   m_users;
    OrderManager                          m_orderManager;
    int                                   m_nextProductId;
    int                                   m_nextUserId;
    std::shared_ptr<User>                m_currentUser; // 当前登录用户

public:
    Store();

    // ======== 初始化 ========
    void initSampleData();

    // ======== 用户管理 ========
    std::shared_ptr<User> login(const std::string& username, const std::string& password);
    std::shared_ptr<User> registerUser(const std::string& username,
                                       const std::string& password, bool isAdmin);
    void logout();
    std::shared_ptr<User> getCurrentUser() const { return m_currentUser; }
    const std::vector<std::shared_ptr<User>>& getAllUsers() const { return m_users; }

    // ======== 商品管理（管理员） ========
    void addProduct(std::shared_ptr<Product> product);
    bool removeProduct(int productId);
    bool restockProduct(int productId, int quantity);
    std::shared_ptr<Product> findProduct(int productId) const;
    const std::vector<std::shared_ptr<Product>>& getAllProducts() const { return m_products; }

    // ======== 顾客操作 ========
    bool addToCart(ShoppingCart& cart, int productId, int quantity) const;
    std::shared_ptr<Order> checkout(ShoppingCart& cart, int customerId,
                                    const std::string& customerName);

    // ======== 订单管理 ========
    OrderManager& getOrderManager() { return m_orderManager; }

    // ======== 显示 ========
    void displayProducts() const;
    void displayProductsByCategory(const std::string& category) const;
};

#endif // STORE_H

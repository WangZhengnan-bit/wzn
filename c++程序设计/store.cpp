#include "store.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// ==================== Store ====================
Store::Store()
    : m_nextProductId(1001), m_nextUserId(1001), m_currentUser(nullptr) {}

void Store::initSampleData() {
    // 预设管理员
    m_users.push_back(std::make_shared<Admin>(m_nextUserId++, "admin", "admin123"));

    // 预设顾客
    m_users.push_back(std::make_shared<Customer>(m_nextUserId++, "user1", "123456"));
    m_users.push_back(std::make_shared<Customer>(m_nextUserId++, "user2", "123456"));

    // 预设商品
    m_products.push_back(std::make_shared<PhysicalProduct>(
        m_nextProductId++, "机械键盘", 299.00, 50, 0.85, "全国", "青轴机械键盘，RGB背光"));
    m_products.push_back(std::make_shared<PhysicalProduct>(
        m_nextProductId++, "无线鼠标", 129.00, 100, 0.12, "全国", "2.4G无线连接，静音按键"));
    m_products.push_back(std::make_shared<PhysicalProduct>(
        m_nextProductId++, "显示器支架", 459.00, 30, 3.50, "全国", "可调节高度，支持双屏"));
    m_products.push_back(std::make_shared<DigitalProduct>(
        m_nextProductId++, "C++编程指南", 49.90, 999, 15.6, "PDF", "C++11/14/17 实战教程"));
    m_products.push_back(std::make_shared<DigitalProduct>(
        m_nextProductId++, "设计素材包", 19.90, 500, 320.0, "ZIP", "1000+ 高清设计素材"));
    m_products.push_back(std::make_shared<PhysicalProduct>(
        m_nextProductId++, "USB-C 扩展坞", 259.00, 45, 0.20, "全国", "7合1，支持4K输出"));
    m_products.push_back(std::make_shared<DigitalProduct>(
        m_nextProductId++, "商业计划书模板", 9.90, 2000, 2.3, "DOCX", "适用于创业融资"));
    m_products.push_back(std::make_shared<PhysicalProduct>(
        m_nextProductId++, "笔记本散热器", 79.00, 60, 0.65, "全国", "双风扇，静音设计"));
}

// ======== 用户管理 ========
std::shared_ptr<User> Store::login(const std::string& username,
                                    const std::string& password) {
    for (auto& user : m_users) {
        if (user->getUsername() == username && user->checkPassword(password)) {
            m_currentUser = user;
            return user;
        }
    }
    return nullptr;
}

std::shared_ptr<User> Store::registerUser(const std::string& username,
                                           const std::string& password,
                                           bool isAdmin) {
    // 检查用户名是否已存在
    for (const auto& user : m_users) {
        if (user->getUsername() == username) {
            std::cout << "  ⚠ 用户名已存在，请换一个。\n";
            return nullptr;
        }
    }

    std::shared_ptr<User> newUser;
    if (isAdmin) {
        newUser = std::make_shared<Admin>(m_nextUserId++, username, password);
    } else {
        newUser = std::make_shared<Customer>(m_nextUserId++, username, password);
    }
    m_users.push_back(newUser);
    m_currentUser = newUser;
    std::cout << "  ✓ 注册成功！欢迎，" << username << "！\n";
    return newUser;
}

void Store::logout() {
    if (m_currentUser) {
        std::cout << "  再见，" << m_currentUser->getUsername() << "！\n";
        m_currentUser = nullptr;
    }
}

// ======== 商品管理 ========
void Store::addProduct(std::shared_ptr<Product> product) {
    product->setId(m_nextProductId++);  // 自动分配ID
    m_products.push_back(std::move(product));
    std::cout << "  ✓ 商品已添加（ID: " << (m_nextProductId - 1) << "）。\n";
}

bool Store::removeProduct(int productId) {
    auto it = std::find_if(m_products.begin(), m_products.end(),
        [productId](const std::shared_ptr<Product>& p) {
            return p->getId() == productId;
        });
    if (it != m_products.end()) {
        std::cout << "  ✓ 已删除商品 \"" << (*it)->getName() << "\"\n";
        m_products.erase(it);
        return true;
    }
    std::cout << "  ✗ 未找到该商品。\n";
    return false;
}

bool Store::restockProduct(int productId, int quantity) {
    auto product = findProduct(productId);
    if (product && quantity > 0) {
        product->increaseStock(quantity);
        std::cout << "  ✓ \"" << product->getName()
                  << "\" 已补货 " << quantity << " 件，当前库存: "
                  << product->getStock() << "\n";
        return true;
    }
    return false;
}

std::shared_ptr<Product> Store::findProduct(int productId) const {
    auto it = std::find_if(m_products.begin(), m_products.end(),
        [productId](const std::shared_ptr<Product>& p) {
            return p->getId() == productId;
        });
    return (it != m_products.end()) ? *it : nullptr;
}

// ======== 顾客操作 ========
bool Store::addToCart(ShoppingCart& cart, int productId, int quantity) const {
    auto product = findProduct(productId);
    if (!product) {
        std::cout << "  ✗ 商品不存在。\n";
        return false;
    }
    if (product->getStock() < quantity) {
        std::cout << "  ✗ 库存不足！当前库存: " << product->getStock() << "\n";
        return false;
    }
    cart.addItem(product, quantity);
    return true;
}

std::shared_ptr<Order> Store::checkout(ShoppingCart& cart, int customerId,
                                        const std::string& customerName) {
    if (cart.isEmpty()) {
        std::cout << "  ✗ 购物车为空，无法结算。\n";
        return nullptr;
    }

    // 扣减库存
    for (const auto& item : cart.getItems()) {
        auto product = findProduct(item.product->getId());
        if (!product || !product->reduceStock(item.quantity)) {
            std::cout << "  ✗ 商品 \"" << item.product->getName()
                      << "\" 库存不足，下单失败。\n";
            // 注意：实际项目中应在此回滚已扣减的库存，这里简化处理
            return nullptr;
        }
    }

    auto order = m_orderManager.createOrder(customerId, customerName,
                                            cart.getItems());
    cart.clear();
    std::cout << "  ✓ 下单成功！订单编号: #" << order->getOrderId() << "\n";
    return order;
}

// ======== 显示 ========
void Store::displayProducts() const {
    if (m_products.empty()) {
        std::cout << "  暂无商品。\n";
        return;
    }
    std::cout << "\n┌──────────────── 商品列表 ─────────────────┐\n";
    for (const auto& product : m_products) {
        product->display();
    }
    std::cout << "└────────────────────────────────────────────┘\n";
}

void Store::displayProductsByCategory(const std::string& category) const {
    std::cout << "\n--- " << category << " ---\n";
    bool found = false;
    for (const auto& product : m_products) {
        if (product->getCategory() == category) {
            product->display();
            found = true;
        }
    }
    if (!found) {
        std::cout << "  该分类暂无商品。\n";
    }
}

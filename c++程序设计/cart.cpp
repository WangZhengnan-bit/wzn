#include "cart.h"
#include "product.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// ==================== ShoppingCart ====================
void ShoppingCart::addItem(std::shared_ptr<Product> product, int quantity) {
    if (quantity <= 0) return;

    // 检查是否已在购物车中
    for (auto& item : m_items) {
        if (item.product->getId() == product->getId()) {
            item.quantity += quantity;
            std::cout << "  已更新 \"" << product->getName()
                      << "\" 数量为 " << item.quantity << "\n";
            return;
        }
    }
    // 新商品
    m_items.emplace_back(std::move(product), quantity);
    std::cout << "  已添加 \"" << m_items.back().product->getName()
              << "\" x" << quantity << " 到购物车\n";
}

void ShoppingCart::removeItem(int productId) {
    auto it = std::find_if(m_items.begin(), m_items.end(),
        [productId](const CartItem& item) {
            return item.product->getId() == productId;
        });
    if (it != m_items.end()) {
        std::cout << "  已从购物车移除 \"" << it->product->getName() << "\"\n";
        m_items.erase(it);
    } else {
        std::cout << "  购物车中未找到该商品。\n";
    }
}

void ShoppingCart::updateQuantity(int productId, int newQuantity) {
    if (newQuantity <= 0) {
        removeItem(productId);
        return;
    }
    for (auto& item : m_items) {
        if (item.product->getId() == productId) {
            item.quantity = newQuantity;
            std::cout << "  已更新 \"" << item.product->getName()
                      << "\" 数量为 " << newQuantity << "\n";
            return;
        }
    }
    std::cout << "  购物车中未找到该商品。\n";
}

void ShoppingCart::clear() {
    m_items.clear();
}

double ShoppingCart::getTotal() const {
    double total = 0.0;
    for (const auto& item : m_items) {
        total += item.product->getPrice() * item.quantity;
    }
    return total;
}

void ShoppingCart::display() const {
    if (m_items.empty()) {
        std::cout << "  购物车为空。\n";
        return;
    }
    std::cout << "\n┌──────────────── 购物车 ────────────────┐\n";
    std::cout << "│ 序号  商品名称            单价      数量  小计\n";
    std::cout << "├──────────────────────────────────────────┤\n";
    int index = 1;
    for (const auto& item : m_items) {
        double subtotal = item.product->getPrice() * item.quantity;
        std::cout << "│ " << std::setw(2) << index++ << ".  "
                  << std::setw(20) << std::left << item.product->getName()
                  << std::right
                  << " ¥" << std::setw(7) << std::fixed << std::setprecision(2) << item.product->getPrice()
                  << " x" << std::setw(3) << item.quantity
                  << " ¥" << std::setw(8) << subtotal << "\n";
    }
    std::cout << "├──────────────────────────────────────────┤\n";
    std::cout << "│ 合计:                           ¥"
              << std::setw(8) << getTotal() << "\n";
    std::cout << "└──────────────────────────────────────────┘\n";
}

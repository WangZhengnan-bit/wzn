#include "product.h"
#include <iostream>
#include <iomanip>

// ==================== Product 基类 ====================
Product::Product(int id, const std::string& name, double price, int stock,
                 const std::string& description)
    : m_id(id), m_name(name), m_price(price), m_stock(stock),
      m_description(description) {}

void Product::display() const {
    std::cout << "[" << m_id << "] " << m_name
              << " | ¥" << std::fixed << std::setprecision(2) << m_price
              << " | 库存: " << m_stock;
    if (!m_description.empty()) {
        std::cout << " | " << m_description;
    }
    std::cout << "\n";
}

bool Product::reduceStock(int quantity) {
    if (quantity <= 0 || m_stock < quantity) {
        return false;
    }
    m_stock -= quantity;
    return true;
}

void Product::increaseStock(int quantity) {
    if (quantity > 0) {
        m_stock += quantity;
    }
}

// ==================== PhysicalProduct ====================
PhysicalProduct::PhysicalProduct(int id, const std::string& name, double price,
                                 int stock, double weight,
                                 const std::string& shippingRegion,
                                 const std::string& description)
    : Product(id, name, price, stock, description),
      m_weight(weight), m_shippingRegion(shippingRegion) {}

void PhysicalProduct::display() const {
    Product::display();
    std::cout << "   [实体商品] 重量: " << m_weight << "kg"
              << ", 配送区域: " << m_shippingRegion << "\n";
}

std::string PhysicalProduct::getCategory() const {
    return "实体商品";
}

// ==================== DigitalProduct ====================
DigitalProduct::DigitalProduct(int id, const std::string& name, double price,
                               int stock, double fileSizeMB,
                               const std::string& fileFormat,
                               const std::string& description)
    : Product(id, name, price, stock, description),
      m_fileSizeMB(fileSizeMB), m_fileFormat(fileFormat) {}

void DigitalProduct::display() const {
    Product::display();
    std::cout << "   [数字商品] 大小: " << m_fileSizeMB << "MB"
              << ", 格式: " << m_fileFormat << "\n";
}

std::string DigitalProduct::getCategory() const {
    return "数字商品";
}

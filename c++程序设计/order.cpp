#include "order.h"
#include "product.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <algorithm>

// ==================== Order ====================
Order::Order(int orderId, int customerId, const std::string& customerName,
             const std::vector<CartItem>& items)
    : m_orderId(orderId), m_customerId(customerId), m_customerName(customerName),
      m_items(items), m_status(OrderStatus::Pending)
{
    m_total = 0.0;
    for (const auto& item : m_items) {
        m_total += item.product->getPrice() * item.quantity;
    }

    // 生成时间字符串
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    std::ostringstream oss;
    oss << (localTime->tm_year + 1900) << "-"
        << std::setw(2) << std::setfill('0') << (localTime->tm_mon + 1) << "-"
        << std::setw(2) << std::setfill('0') << localTime->tm_mday << " "
        << std::setw(2) << std::setfill('0') << localTime->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << localTime->tm_min << ":"
        << std::setw(2) << std::setfill('0') << localTime->tm_sec;
    m_orderTime = oss.str();
}

void Order::setStatus(OrderStatus status) {
    m_status = status;
}

std::string Order::getStatusString() const {
    return statusToString(m_status);
}

std::string Order::statusToString(OrderStatus s) {
    switch (s) {
        case OrderStatus::Pending:   return "待处理";
        case OrderStatus::Confirmed: return "已确认";
        case OrderStatus::Shipped:   return "已发货";
        case OrderStatus::Delivered: return "已签收";
        case OrderStatus::Cancelled: return "已取消";
        default:                     return "未知";
    }
}

void Order::display() const {
    std::cout << "\n┌──────────────── 订单详情 ────────────────┐\n";
    std::cout << "│ 订单编号: #" << m_orderId << "\n";
    std::cout << "│ 顾客:     " << m_customerName << " (ID:" << m_customerId << ")\n";
    std::cout << "│ 下单时间: " << m_orderTime << "\n";
    std::cout << "│ 状态:     " << getStatusString() << "\n";
    std::cout << "├──────────────────────────────────────────┤\n";
    for (const auto& item : m_items) {
        double subtotal = item.product->getPrice() * item.quantity;
        std::cout << "│ · " << std::setw(22) << std::left << item.product->getName()
                  << std::right << " x" << item.quantity
                  << "  ¥" << std::fixed << std::setprecision(2) << subtotal << "\n";
    }
    std::cout << "├──────────────────────────────────────────┤\n";
    std::cout << "│ 合计: ¥" << std::fixed << std::setprecision(2) << m_total << "\n";
    std::cout << "└──────────────────────────────────────────┘\n";
}

// ==================== OrderManager ====================
OrderManager::OrderManager() : m_nextOrderId(10001) {}

std::shared_ptr<Order> OrderManager::createOrder(
    int customerId, const std::string& customerName,
    const std::vector<CartItem>& items)
{
    auto order = std::make_shared<Order>(m_nextOrderId++, customerId,
                                         customerName, items);
    m_orders.push_back(order);
    return order;
}

bool OrderManager::cancelOrder(int orderId) {
    auto order = findOrder(orderId);
    if (!order) return false;
    if (order->getStatus() == OrderStatus::Delivered ||
        order->getStatus() == OrderStatus::Cancelled) {
        return false;
    }
    // 恢复库存
    for (const auto& item : order->getItems()) {
        item.product->increaseStock(item.quantity);
    }
    order->setStatus(OrderStatus::Cancelled);
    return true;
}

bool OrderManager::updateOrderStatus(int orderId, OrderStatus newStatus) {
    auto order = findOrder(orderId);
    if (!order) return false;
    order->setStatus(newStatus);
    return true;
}

std::shared_ptr<Order> OrderManager::findOrder(int orderId) const {
    auto it = std::find_if(m_orders.begin(), m_orders.end(),
        [orderId](const std::shared_ptr<Order>& o) {
            return o->getOrderId() == orderId;
        });
    return (it != m_orders.end()) ? *it : nullptr;
}

std::vector<std::shared_ptr<Order>> OrderManager::getOrdersByCustomer(
    int customerId) const
{
    std::vector<std::shared_ptr<Order>> result;
    for (const auto& order : m_orders) {
        if (order->getCustomerId() == customerId) {
            result.push_back(order);
        }
    }
    return result;
}

void OrderManager::displayAllOrders() const {
    if (m_orders.empty()) {
        std::cout << "  暂无订单。\n";
        return;
    }
    std::cout << "\n┌──────────────── 所有订单 ─────────────────┐\n";
    for (const auto& order : m_orders) {
        std::cout << "│ #" << order->getOrderId()
                  << " | " << order->getCustomerName()
                  << " | ¥" << std::fixed << std::setprecision(2) << order->getTotal()
                  << " | " << order->getStatusString() << "\n";
    }
    std::cout << "└────────────────────────────────────────────┘\n";
}

void OrderManager::displayOrdersByCustomer(int customerId) const {
    auto orders = getOrdersByCustomer(customerId);
    if (orders.empty()) {
        std::cout << "  该顾客暂无订单。\n";
        return;
    }
    for (const auto& order : orders) {
        order->display();
    }
}

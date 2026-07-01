#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <memory>
#include "cart.h"

// ==================== 订单状态 ====================
enum class OrderStatus {
    Pending,     // 待处理
    Confirmed,   // 已确认
    Shipped,     // 已发货
    Delivered,   // 已签收
    Cancelled    // 已取消
};

// ==================== 订单类 ====================
class Order {
private:
    int                         m_orderId;
    int                         m_customerId;
    std::string                 m_customerName;
    std::vector<CartItem>       m_items;
    double                      m_total;
    OrderStatus                 m_status;
    std::string                 m_orderTime;

public:
    Order(int orderId, int customerId, const std::string& customerName,
          const std::vector<CartItem>& items);

    // ---- 查询 ----
    int                          getOrderId()     const { return m_orderId; }
    int                          getCustomerId()  const { return m_customerId; }
    const std::string&           getCustomerName()const { return m_customerName; }
    double                       getTotal()       const { return m_total; }
    OrderStatus                  getStatus()      const { return m_status; }
    const std::string&           getOrderTime()   const { return m_orderTime; }
    const std::vector<CartItem>& getItems()       const { return m_items; }

    // ---- 状态管理 ----
    void setStatus(OrderStatus status);
    std::string getStatusString() const;
    static std::string statusToString(OrderStatus s);

    // ---- 显示 ----
    void display() const;
};

// ==================== 订单管理器 ====================
class OrderManager {
private:
    std::vector<std::shared_ptr<Order>> m_orders;
    int                                 m_nextOrderId;

public:
    OrderManager();

    // ---- 订单操作 ----
    std::shared_ptr<Order> createOrder(int customerId, const std::string& customerName,
                                       const std::vector<CartItem>& items);
    bool cancelOrder(int orderId);
    bool updateOrderStatus(int orderId, OrderStatus newStatus);

    // ---- 查询 ----
    std::shared_ptr<Order>                    findOrder(int orderId) const;
    std::vector<std::shared_ptr<Order>>       getOrdersByCustomer(int customerId) const;
    const std::vector<std::shared_ptr<Order>>& getAllOrders() const { return m_orders; }
    int getOrderCount() const { return static_cast<int>(m_orders.size()); }

    // ---- 显示 ----
    void displayAllOrders() const;
    void displayOrdersByCustomer(int customerId) const;
};

#endif // ORDER_H

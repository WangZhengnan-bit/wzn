#ifndef CART_H
#define CART_H

#include <vector>
#include <memory>

class Product;

// ==================== 购物车条目 ====================
struct CartItem {
    std::shared_ptr<Product> product;
    int                      quantity;

    CartItem(std::shared_ptr<Product> p, int qty)
        : product(std::move(p)), quantity(qty) {}
};

// ==================== 购物车 ====================
class ShoppingCart {
private:
    std::vector<CartItem> m_items;

public:
    ShoppingCart() = default;

    // ---- 购物车操作 ----
    void addItem(std::shared_ptr<Product> product, int quantity);
    void removeItem(int productId);
    void updateQuantity(int productId, int newQuantity);
    void clear();

    // ---- 查询 ----
    double                          getTotal()     const;
    int                             getItemCount() const { return static_cast<int>(m_items.size()); }
    bool                            isEmpty()      const { return m_items.empty(); }
    const std::vector<CartItem>&    getItems()     const { return m_items; }

    // ---- 显示 ----
    void display() const;
};

#endif // CART_H

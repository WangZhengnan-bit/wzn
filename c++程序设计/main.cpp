/**
 * 电子商务系统 — 购物车、订单管理、商品库存模拟
 *
 * 面向对象设计：
 *   User  ──┬── Admin      (管理员：管理商品、处理订单)
 *           └── Customer   (顾客：浏览商品、购物车、下单)
 *   Product──┬── PhysicalProduct (实体商品：含重量、配送信息)
 *            └── DigitalProduct  (数字商品：含文件大小、格式)
 *
 * 组合类：ShoppingCart, Order, OrderManager, Store
 *
 * 编译方式 (g++)：
 *   g++ -std=c++11 -Wall -o ecommerce.exe main.cpp user.cpp product.cpp cart.cpp order.cpp store.cpp
 */

#include <iostream>
#include <string>
#include <limits>
#include <memory>
#include "store.h"

// ==================== 辅助函数 ====================
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    std::cout << "\n按 Enter 键继续...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printTitle(const std::string& title) {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════╗\n";
    std::cout << "║  " << title;
    for (size_t i = title.size(); i < 40; i += 2) {
        std::cout << "  ";
    }
    std::cout << "\n";
    std::cout << "╚════════════════════════════════════════════╝\n";
}

int readInt(const std::string& prompt = "请输入选项: ") {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  ⚠ 请输入有效数字。\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value < 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  ⚠ 请输入有效数字。\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

std::string readString(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

// ==================== 管理员菜单 ====================
void adminMenu(Store& store) {
    while (true) {
        clearScreen();
        auto user = store.getCurrentUser();
        user->display();

        std::cout << "\n┌──────────────── 管理员菜单 ────────────────┐\n";
        std::cout << "│ 1. 查看所有商品                              │\n";
        std::cout << "│ 2. 添加商品（实体/数字）                       │\n";
        std::cout << "│ 3. 删除商品                                  │\n";
        std::cout << "│ 4. 补货商品                                  │\n";
        std::cout << "│ 5. 查看所有订单                              │\n";
        std::cout << "│ 6. 更新订单状态                              │\n";
        std::cout << "│ 7. 取消订单（退款+恢复库存）                   │\n";
        std::cout << "│ 0. 退出登录                                  │\n";
        std::cout << "└──────────────────────────────────────────────┘\n";

        int choice = readInt();

        switch (choice) {
        case 1: { // 查看商品
            clearScreen();
            printTitle("商品列表");
            store.displayProducts();
            pauseScreen();
            break;
        }
        case 2: { // 添加商品
            clearScreen();
            printTitle("添加商品");

            std::cout << "选择商品类型:\n";
            std::cout << "  1. 实体商品\n";
            std::cout << "  2. 数字商品\n";
            int type = readInt("请选择: ");

            std::string name = readString("商品名称: ");
            double price = readDouble("价格 (¥): ");
            int stock = readInt("初始库存: ");
            std::string desc = readString("描述 (可选): ");

            if (type == 1) {
                double weight = readDouble("重量 (kg): ");
                std::string region = readString("配送区域: ");
                auto p = std::make_shared<PhysicalProduct>(
                    0, name, price, stock, weight, region, desc);
                store.addProduct(p);
            } else {
                double size = readDouble("文件大小 (MB): ");
                std::string format = readString("文件格式: ");
                auto p = std::make_shared<DigitalProduct>(
                    0, name, price, stock, size, format, desc);
                store.addProduct(p);
            }
            pauseScreen();
            break;
        }
        case 3: { // 删除商品
            clearScreen();
            printTitle("删除商品");
            store.displayProducts();
            int id = readInt("输入要删除的商品ID: ");
            store.removeProduct(id);
            pauseScreen();
            break;
        }
        case 4: { // 补货
            clearScreen();
            printTitle("补货商品");
            store.displayProducts();
            int id = readInt("输入商品ID: ");
            int qty = readInt("补货数量: ");
            store.restockProduct(id, qty);
            pauseScreen();
            break;
        }
        case 5: { // 查看所有订单
            clearScreen();
            printTitle("所有订单");
            store.getOrderManager().displayAllOrders();
            pauseScreen();
            break;
        }
        case 6: { // 更新订单状态
            clearScreen();
            printTitle("更新订单状态");
            store.getOrderManager().displayAllOrders();
            int orderId = readInt("输入订单编号: ");
            auto order = store.getOrderManager().findOrder(orderId);
            if (!order) {
                std::cout << "  ✗ 订单不存在。\n";
            } else {
                std::cout << "当前状态: " << order->getStatusString() << "\n";
                std::cout << "可选状态:\n";
                std::cout << "  1. 已确认  2. 已发货  3. 已签收  4. 已取消\n";
                int s = readInt("选择新状态: ");
                OrderStatus statuses[] = {OrderStatus::Confirmed, OrderStatus::Shipped,
                                          OrderStatus::Delivered, OrderStatus::Cancelled};
                if (s >= 1 && s <= 4) {
                    store.getOrderManager().updateOrderStatus(orderId, statuses[s - 1]);
                    std::cout << "  ✓ 状态已更新。\n";
                } else {
                    std::cout << "  ✗ 无效选择。\n";
                }
            }
            pauseScreen();
            break;
        }
        case 7: { // 取消订单
            clearScreen();
            printTitle("取消订单");
            store.getOrderManager().displayAllOrders();
            int orderId = readInt("输入要取消的订单编号: ");
            if (store.getOrderManager().cancelOrder(orderId)) {
                std::cout << "  ✓ 订单已取消，库存已恢复。\n";
            } else {
                std::cout << "  ✗ 取消失败（订单不存在或不可取消）。\n";
            }
            pauseScreen();
            break;
        }
        case 0:
            store.logout();
            return;
        default:
            std::cout << "  ⚠ 无效选项，请重新选择。\n";
            pauseScreen();
        }
    }
}

// ==================== 顾客菜单 ====================
void customerMenu(Store& store) {
    ShoppingCart cart;
    auto customer = std::dynamic_pointer_cast<Customer>(store.getCurrentUser());

    while (true) {
        clearScreen();
        auto user = store.getCurrentUser();
        user->display();

        std::cout << "\n┌──────────────── 顾客菜单 ───────────────────┐\n";
        std::cout << "│ 1. 浏览所有商品                              │\n";
        std::cout << "│ 2. 按分类浏览商品                            │\n";
        std::cout << "│ 3. 添加商品到购物车                          │\n";
        std::cout << "│ 4. 查看购物车                                │\n";
        std::cout << "│ 5. 修改购物车商品数量                         │\n";
        std::cout << "│ 6. 从购物车移除商品                           │\n";
        std::cout << "│ 7. 结算下单                                  │\n";
        std::cout << "│ 8. 查看我的订单                              │\n";
        std::cout << "│ 0. 退出登录                                  │\n";
        std::cout << "└──────────────────────────────────────────────┘\n";

        int choice = readInt();

        switch (choice) {
        case 1: { // 浏览所有商品
            clearScreen();
            printTitle("所有商品");
            store.displayProducts();
            pauseScreen();
            break;
        }
        case 2: { // 按分类浏览
            clearScreen();
            printTitle("按分类浏览");
            std::cout << "  1. 实体商品\n";
            std::cout << "  2. 数字商品\n";
            int c = readInt("选择分类: ");
            if (c == 1) {
                store.displayProductsByCategory("实体商品");
            } else if (c == 2) {
                store.displayProductsByCategory("数字商品");
            } else {
                std::cout << "  ⚠ 无效选择。\n";
            }
            pauseScreen();
            break;
        }
        case 3: { // 添加到购物车
            clearScreen();
            printTitle("添加到购物车");
            store.displayProducts();
            int id = readInt("输入商品ID: ");
            int qty = readInt("数量: ");
            store.addToCart(cart, id, qty);
            pauseScreen();
            break;
        }
        case 4: { // 查看购物车
            clearScreen();
            printTitle("我的购物车");
            cart.display();
            pauseScreen();
            break;
        }
        case 5: { // 修改数量
            clearScreen();
            printTitle("修改商品数量");
            cart.display();
            if (!cart.isEmpty()) {
                int id = readInt("输入商品ID: ");
                int qty = readInt("新数量: ");
                cart.updateQuantity(id, qty);
            }
            pauseScreen();
            break;
        }
        case 6: { // 移除商品
            clearScreen();
            printTitle("移除商品");
            cart.display();
            if (!cart.isEmpty()) {
                int id = readInt("输入要移除的商品ID: ");
                cart.removeItem(id);
            }
            pauseScreen();
            break;
        }
        case 7: { // 结算
            clearScreen();
            printTitle("结算下单");
            cart.display();
            if (!cart.isEmpty()) {
                std::cout << "确认下单？(y/n): ";
                std::string confirm;
                std::getline(std::cin, confirm);
                if (confirm == "y" || confirm == "Y") {
                    auto order = store.checkout(cart, customer->getId(),
                                                customer->getUsername());
                    if (order) {
                        customer->addOrder(order);
                    }
                } else {
                    std::cout << "  已取消。\n";
                }
            }
            pauseScreen();
            break;
        }
        case 8: { // 查看我的订单
            clearScreen();
            printTitle("我的订单");
            customer->viewOrderHistory();
            pauseScreen();
            break;
        }
        case 0:
            store.logout();
            return;
        default:
            std::cout << "  ⚠ 无效选项，请重新选择。\n";
            pauseScreen();
        }
    }
}

// ==================== 主菜单 ====================
void mainMenu(Store& store) {
    while (true) {
        clearScreen();
        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════════╗\n";
        std::cout << "║        🛒 电子商务管理系统 v1.0           ║\n";
        std::cout << "║   购物车 · 订单管理 · 商品库存模拟         ║\n";
        std::cout << "╠════════════════════════════════════════════╣\n";
        std::cout << "║                                            ║\n";
        std::cout << "║    1. 登录                                 ║\n";
        std::cout << "║    2. 注册新用户                           ║\n";
        std::cout << "║    0. 退出系统                             ║\n";
        std::cout << "║                                            ║\n";
        std::cout << "╚════════════════════════════════════════════╝\n";

        int choice = readInt();

        switch (choice) {
        case 1: { // 登录
            clearScreen();
            printTitle("用户登录");
            std::string username = readString("用户名: ");
            std::string password = readString("密  码: ");

            auto user = store.login(username, password);
            if (user) {
                std::cout << "  ✓ 登录成功！\n";
                pauseScreen();
                if (user->getRole() == "管理员") {
                    adminMenu(store);
                } else {
                    customerMenu(store);
                }
            } else {
                std::cout << "  ✗ 用户名或密码错误。\n";
                pauseScreen();
            }
            break;
        }
        case 2: { // 注册
            clearScreen();
            printTitle("注册新用户");
            std::string username = readString("用户名: ");
            std::string password = readString("密  码: ");
            std::cout << "注册类型 (1=顾客, 2=管理员): ";
            int type;
            std::cin >> type;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            auto newUser = store.registerUser(username, password, type == 2);
            if (newUser) {
                pauseScreen();
                if (newUser->getRole() == "管理员") {
                    adminMenu(store);
                } else {
                    customerMenu(store);
                }
            } else {
                pauseScreen();
            }
            break;
        }
        case 0:
            std::cout << "\n  感谢使用！再见！\n";
            return;
        default:
            std::cout << "  ⚠ 无效选项。\n";
            pauseScreen();
        }
    }
}

// ==================== 程序入口 ====================
int main() {
    // 设置控制台编码为 UTF-8 (Windows)
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    Store store;
    store.initSampleData();

    mainMenu(store);
    return 0;
}

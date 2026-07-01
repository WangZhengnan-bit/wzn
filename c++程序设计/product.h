#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

// ==================== 商品基类 ====================
class Product {
protected:
    int         m_id;
    std::string m_name;
    double      m_price;
    int         m_stock;
    std::string m_description;

public:
    Product(int id, const std::string& name, double price, int stock,
            const std::string& description = "");
    virtual ~Product() = default;

    // ---- 虚函数：派生类重写 ----
    virtual void        display()     const;
    virtual std::string getCategory() const = 0; // 纯虚函数

    // ---- 公共访问器 ----
    int                getId()          const { return m_id; }
    const std::string& getName()        const { return m_name; }
    double             getPrice()       const { return m_price; }
    int                getStock()       const { return m_stock; }
    const std::string& getDescription() const { return m_description; }

    // ---- 属性设置 ----
    void setId(int id)     { m_id = id; }
    void setStock(int stock) { m_stock = stock; }
    bool reduceStock(int quantity);   // 扣减库存，true=成功
    void increaseStock(int quantity); // 增加库存
};

// ==================== 实体商品 ====================
class PhysicalProduct : public Product {
private:
    double      m_weight;         // 重量（kg）
    std::string m_shippingRegion; // 配送区域

public:
    PhysicalProduct(int id, const std::string& name, double price, int stock,
                    double weight, const std::string& shippingRegion = "全国",
                    const std::string& description = "");

    void        display()     const override;
    std::string getCategory() const override;

    double             getWeight()        const { return m_weight; }
    const std::string& getShippingRegion() const { return m_shippingRegion; }
};

// ==================== 数字商品 ====================
class DigitalProduct : public Product {
private:
    double      m_fileSizeMB;   // 文件大小
    std::string m_fileFormat;   // 文件格式（如 PDF, MP3, ZIP）

public:
    DigitalProduct(int id, const std::string& name, double price, int stock,
                   double fileSizeMB, const std::string& fileFormat = "通用",
                   const std::string& description = "");

    void        display()     const override;
    std::string getCategory() const override;

    double             getFileSize()   const { return m_fileSizeMB; }
    const std::string& getFileFormat() const { return m_fileFormat; }
};

#endif // PRODUCT_H


#pragma once
#include <iostream>
#include <string>

// TEMPORARY itemtype class, for now we may only use weapon or consumable, but this is for future expansion
enum class ItemType {
    Weapon,
    Consumable,
    Resource,
    Armor
};

class Item {
    private:
        std::string name;
        ItemType type;
        int quantity;

    public:
        Item(const std::string& n="", ItemType type = ItemType::Resource, int quant=1);
        std::string GetName() const;
        ItemType GetType() const;
        int GetQuantity() const;
        void AddQuantity(int amount);
        void RemoveQuantity(int amount);
};

class Inventory {
    private:
        struct Node {
            Item data;
            Node* next;
            Node(const Item& item);
        };
        Node* head;

    public:
        Inventory();
        void AddItem(const Item& item);
        // Remove current item from linked list
        bool RemoveItem(const std::string& itemName, int amount = 1);
        // Basic LL existence check
        bool HasItem(const std::string& itemName) const;
        // Basic LL printing
        void PrintInventory() const;
        // Clearing list, also use for deletion
        void Clear();
        ~Inventory();
};
#include "inventory.h"

Item::Item(const std::string& n, ItemType type, int quant) : name(n), type(type), quantity(quant) {}

std::string Item::GetName() const {
    return name;
}

ItemType Item::GetType() const {
    return type;
}

int Item::GetQuantity() const {
    return quantity;
}

void Item::AddQuantity(int amount) {
    quantity += amount;
}

void Item::RemoveQuantity(int amount) {
    quantity -= amount;
    if (quantity < 0) {quantity = 0;}
}

Inventory::Node::Node(const Item& item) : data(item), next(nullptr) {}

Inventory::Inventory() : head(nullptr) {}

void Inventory::AddItem(const Item& item) {
    Node* current = head;

    while (current != nullptr) {
        if (current->data.GetName() == item.GetName()) {
            current->data.AddQuantity(item.GetQuantity());
            return;
        }
        current = current->next;
    }

    Node* newNode = new Node(item);
    newNode->next = head;
    head = newNode;
}

// Remove current item from linked list
bool Inventory::RemoveItem(const std::string& itemName, int amount) {
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->data.GetName() == itemName) {
            current->data.RemoveQuantity(amount);
            if (current->data.GetQuantity() <= 0) {
                if (previous == nullptr) {
                    head = current->next;
                }
                else {
                    previous->next = current->next;
                }
                delete current;
            }
            return true;
        }
        previous = current;
        current = current->next;
    }

    return false;
}

// Basic LL existence check
bool Inventory::HasItem(const std::string& itemName) const {
    Node* current = head;

    while (current != nullptr) {
        if (current->data.GetName() == itemName)
            return true;

        current = current->next;
    }

    return false;
}

// Basic LL printing
void Inventory::PrintInventory() const {
    if (head == nullptr) {std::cout << "Inventory Empty" << std::endl; return;}
    Node* current = head;

    while (current != nullptr) {
        std::cout << current->data.GetName() << " x" << current->data.GetQuantity() << std::endl;
        current = current->next;
    }
}

// Clearing list, also use for deletion
void Inventory::Clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }

    head = nullptr;
}

Inventory::~Inventory() {
    Clear();
}
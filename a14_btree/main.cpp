#include <iostream>
#include "../include/btree.h"

void test();

int main() {
    test();

    return 0;
}

void test() {
    using namespace btree;

    BTree<int> bt;
    int insert;

    std::cout << "Empty btree:\n";
    std::cout << bt << std::endl << std::endl;

    insert = 10;
    bt.insert(insert);
    std::cout << "insert: " << insert << std::endl;
    std::cout << bt << std::endl << std::endl;

    insert = 20;
    bt.insert(insert);
    std::cout << "insert: " << insert << std::endl;
    std::cout << bt << std::endl << std::endl;

    insert = 30;
    bt.insert(insert);
    std::cout << "insert: " << insert << std::endl;
    std::cout << bt << std::endl << std::endl;

    insert = 40;
    bt.insert(insert);
    std::cout << "insert: " << insert << std::endl;
    std::cout << bt << std::endl << std::endl;

    insert = 50;
    bt.insert(insert);
    std::cout << "insert: " << insert << std::endl;
    std::cout << bt << std::endl << std::endl;

    insert = 60;
    bt.insert(insert);
    std::cout << "insert: " << insert << std::endl;
    std::cout << bt << std::endl << std::endl;

    insert = 70;
    bt.insert(insert);
    std::cout << "insert: " << insert << std::endl;
    std::cout << bt << std::endl << std::endl;

    insert = 80;
    bt.insert(insert);
    std::cout << "insert: " << insert << std::endl;
    std::cout << bt << std::endl << std::endl;

    insert = 90;
    bt.insert(insert);
    std::cout << "insert: " << insert << std::endl;
    std::cout << bt << std::endl << std::endl;

    insert = 100;
    bt.insert(insert);
    std::cout << "insert: " << insert << std::endl;
    std::cout << bt << std::endl << std::endl;

    insert = 110;
    bt.insert(insert);
    std::cout << "insert: " << insert << std::endl;
    std::cout << bt << std::endl << std::endl;

    insert = 120;
    bt.insert(insert);
    std::cout << "insert: " << insert << std::endl;
    std::cout << bt << std::endl << std::endl;

    std::cout << "bt empty = " << bt.empty() << std::endl;
    std::cout << "bt size = " << bt.size() << std::endl;

    BTree<int> bt2;
    bt2.copy_tree(bt);
    std::cout << "\nbt2 is copied with bt\n";
    std::cout << bt2 << std::endl;

    std::cout << "bt2 empty = " << bt2.empty() << std::endl;
    std::cout << "bt2 size = " << bt2.size() << std::endl;

    BTree<int> bt3(bt);

    std::cout << "\nbt3 is copy ctor\n";
    std::cout << bt3 << std::endl;

    std::cout << "bt3 empty = " << bt3.empty() << std::endl;
    std::cout << "bt3 size = " << bt3.size() << std::endl;

    std::cout << "bt4: " << std::endl;
    BTree<int> bt4;
    bt4.insert(30);
    bt4.insert(80);
    std::cout << bt4 << std::endl;

    std::cout << "bt4 empty = " << bt4.empty() << std::endl;
    std::cout << "bt4 size = " << bt4.size() << std::endl;

    bt4 = bt;

    std::cout << "assign op\n";
    std::cout << bt4 << std::endl;

    std::cout << "bt4 empty = " << bt4.empty() << std::endl;
    std::cout << "bt4 size = " << bt4.size() << std::endl;

    bool is_found = false;
    int search = -999;
    int entry = -999;

    is_found = false;
    search = 10;
    is_found = bt.contains(search);
    std::cout << "is_found = " << is_found << std::endl;
    entry = bt.get(search);
    std::cout << "got is " << entry << std::endl;

    is_found = false;
    search = 20;
    is_found = bt.contains(search);
    std::cout << "is_found = " << is_found << std::endl;
    entry = bt.get(search);
    std::cout << "got is " << entry << std::endl;

    is_found = false;
    search = 30;
    is_found = bt.contains(search);
    std::cout << "is_found = " << is_found << std::endl;
    entry = bt.get(search);
    std::cout << "got is " << entry << std::endl;

    is_found = false;
    search = 60;
    is_found = bt.contains(search);
    std::cout << "is_found = " << is_found << std::endl;
    entry = bt.get(search);
    std::cout << "got is " << entry << std::endl;

    is_found = false;
    search = 100;
    is_found = bt.contains(search);
    std::cout << "is_found = " << is_found << std::endl;
    entry = bt.get(search);
    std::cout << "got is " << entry << std::endl;

    is_found = false;
    search = 110;
    is_found = bt.contains(search);
    std::cout << "is_found = " << is_found << std::endl;
    entry = bt.get(search);
    std::cout << "got is " << entry << std::endl;

    is_found = false;
    search = 120;
    is_found = bt.contains(search);
    std::cout << "is_found = " << is_found << std::endl;
    entry = bt.get(search);
    std::cout << "got is " << entry << std::endl;

    std::cout << "starting on invalid entries" << std::endl;

    is_found = true;
    search = -1;
    is_found = bt.contains(search);
    std::cout << "is_found = " << is_found << std::endl;

    is_found = true;
    search = -0;
    is_found = bt.contains(search);
    std::cout << "is_found = " << is_found << std::endl;

    is_found = true;
    search = 15;
    is_found = bt.contains(search);
    std::cout << "is_found = " << is_found << std::endl;

    is_found = true;
    search = 55;
    is_found = bt.contains(search);
    std::cout << "is_found = " << is_found << std::endl;

    is_found = true;
    search = 115;
    is_found = bt.contains(search);
    std::cout << "is_found = " << is_found << std::endl;

    is_found = true;
    search = 130;
    is_found = bt.contains(search);
    std::cout << "is_found = " << is_found << std::endl;

    std::cout << "starting get on illegal entries" << std::endl;

    search = -1;
    try {
        std::cout << "trying to get invalid entry: " << search << std::endl;
        entry = bt.get(search);
    } catch(const std::invalid_argument& ia) {
        std::cout << "Invalid argument: " << ia.what() << std::endl;
    }

    search = 0;
    try {
        std::cout << "trying to get invalid entry: " << search << std::endl;
        entry = bt.get(search);
    } catch(const std::invalid_argument& ia) {
        std::cout << "Invalid argument: " << ia.what() << std::endl;
    }

    search = 15;
    try {
        std::cout << "trying to get invalid entry: " << search << std::endl;
        entry = bt.get(search);
    } catch(const std::invalid_argument& ia) {
        std::cout << "Invalid argument: " << ia.what() << std::endl;
    }

    search = 135;
    try {
        std::cout << "trying to get invalid entry: " << search << std::endl;
        entry = bt.get(search);
    } catch(const std::invalid_argument& ia) {
        std::cout << "Invalid argument: " << ia.what() << std::endl;
    }

    std::cout << "\nStarting find pointer" << std::endl;

    int* found = nullptr;

    found = nullptr;
    search = 10;
    found = bt.find(search);
    if(found)
        std::cout << "found = " << *found << std::endl;
    else
        std::cout << "can not find = " << search << std::endl;

    found = nullptr;
    search = 20;
    found = bt.find(search);
    if(found)
        std::cout << "found = " << *found << std::endl;
    else
        std::cout << "can not find = " << search << std::endl;

    found = nullptr;
    search = 30;
    found = bt.find(search);
    if(found)
        std::cout << "found = " << *found << std::endl;
    else
        std::cout << "can not find = " << search << std::endl;

    found = nullptr;
    search = 60;
    found = bt.find(search);
    if(found)
        std::cout << "found = " << *found << std::endl;
    else
        std::cout << "can not find = " << search << std::endl;

    found = nullptr;
    search = 100;
    found = bt.find(search);
    if(found)
        std::cout << "found = " << *found << std::endl;
    else
        std::cout << "can not find = " << search << std::endl;

    found = nullptr;
    search = 110;
    found = bt.find(search);
    if(found)
        std::cout << "found = " << *found << std::endl;
    else
        std::cout << "can not find = " << search << std::endl;

    found = nullptr;
    search = 120;
    found = bt.find(search);
    if(found)
        std::cout << "found = " << *found << std::endl;
    else
        std::cout << "can not find = " << search << std::endl;

    std::cout << "starting invalid" << std::endl;

    found = nullptr;
    search = -1;
    found = bt.find(search);
    if(found)
        std::cout << "found = " << *found << std::endl;
    else
        std::cout << "can not find = " << search << std::endl;

    found = nullptr;
    search = 0;
    found = bt.find(search);
    if(found)
        std::cout << "found = " << *found << std::endl;
    else
        std::cout << "can not find = " << search << std::endl;

    found = nullptr;
    search = 15;
    found = bt.find(search);
    if(found)
        std::cout << "found = " << *found << std::endl;
    else
        std::cout << "can not find = " << search << std::endl;

    found = nullptr;
    search = 45;
    found = bt.find(search);
    if(found)
        std::cout << "found = " << *found << std::endl;
    else
        std::cout << "can not find = " << search << std::endl;

    found = nullptr;
    search = 135;
    found = bt.find(search);
    if(found)
        std::cout << "found = " << *found << std::endl;
    else
        std::cout << "can not find = " << search << std::endl;
}

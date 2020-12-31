//
// Created by Andrey Galaktionov on 28.12.2020.
//

#include <string>
#include <utility>
#include <cstdint>
#include "gtest/gtest.h"
#include "tree.h"

TEST(tree, simple) {
    AVLTree<int, int> tree;
    ASSERT_EQ(tree.size(), 0);
}

TEST(tree, put) {
    AVLTree<std::string, int> tree;
    tree.put("armor", 50);
    tree.put("health", 100);
    tree.put("ammo", 255);
    ASSERT_EQ(tree.size(), 3);
}

TEST(tree, remove) {
    AVLTree<std::string, int> tree;
    tree.put("armor", 50);
    tree.put("health", 100);
    tree.put("ammo", 255);
    tree.remove("ammo");
    tree.remove("health");
    ASSERT_TRUE(tree.contains("armor"));
    ASSERT_FALSE(tree.contains("health"));
    ASSERT_FALSE(tree.contains("ammo"));
    ASSERT_EQ(tree.size(), 1);
}

TEST(tree, contains) {
    AVLTree<std::string, int> tree;
    tree.put("armor", 50);
    tree.put("health", 100);
    tree.put("ammo", 255);
    ASSERT_TRUE(tree.contains("ammo"));
    ASSERT_TRUE(tree.contains("health"));
    ASSERT_TRUE(tree.contains("ammo"));
    ASSERT_FALSE(tree.contains("test"));
}

//TEST(tree, size) {
//    AVLTree<std::string, int> tree;
//    tree["armor"] = 50;
//    tree["health"] = 100;
//    tree["ammo"] = 255;
//    ASSERT_EQ(tree.size(), 3);
//}

TEST(tree, big) {
    AVLTree<int, int> tree;
    int N = 100000;

    for (int i = 0; i < N; i += 1) {
        tree.put(i, i);
    }

    ASSERT_EQ(tree.size(), N);
}

TEST(tree, classes) {
    struct Person {
        int age;
        std::string name;

        Person(std::string name, int age) : name(std::move(name)), age(age) {}

        bool operator<(const Person &other) const {
            return age < other.age;
        }

        bool operator>(const Person &other) const {
            return age > other.age;
        }

        bool operator==(const Person &other) const {
            return age == other.age;
        }
    };
    struct Color {
        uint8_t r, g, b;

        Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    };

    AVLTree<Person, Color> tree;
    tree.put(Person("Ivan", 15), Color(0xff, 0xff, 0xff));
    tree.put(Person("Lena", 16), Color(0x00, 0xff, 0x00));
    tree.put(Person("Rick", 23), Color(0x20, 0xff, 0x00));

    ASSERT_TRUE(tree.contains(Person("Ivan", 15)));
    ASSERT_TRUE(tree.contains(Person("Lena", 16)));
    ASSERT_TRUE(tree.contains(Person("Rick", 23)));
    ASSERT_FALSE(tree.contains(Person("Igor", 12)));

    ASSERT_EQ(tree.size(), 3);
}

TEST(tree_iterator, simple) {
    AVLTree<std::string, int> tree;
    tree.put("Bruh", 255);
    tree.put("look", 255);
    tree.put("at", 255);
    tree.put("this", 255);
    tree.put("dude", 255);
    tree.put("Wait", 255);
    tree.put("till", 255);
    tree.put("you", 255);
    tree.put("see", 255);
    tree.put("the", 255);
    tree.put("f-", 255);
    tree.put("oh no no no no oh ho ho", 255);
    tree.put("oh no no no no oh ho ho", 255);
    tree.put("HA HA HA! LOOK AT THE TOP OF HIS HEAD", 255);

    for (auto it = tree.iterator(); !it.isEnd(); it.next()) {
        std::cout << it.key() << it.get() << std::endl;
    }
}


//TEST(tree, )
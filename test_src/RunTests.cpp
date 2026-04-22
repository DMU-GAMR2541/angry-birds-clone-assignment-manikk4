#include <gtest/gtest.h>
#include "Enemy.h"
#include "Slingshot.h"

/// <summary>
///Taken from the GoogleTest primer. 
/// </summary>

// The fixture for testing class Foo.
class EnemyTest : public testing::Test {
public:
    std::unique_ptr<Enemy> enemy;
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    EnemyTest() {
        // You can do set-up work for each test here.
                    
    }

    ~EnemyTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
        enemy = std::make_unique<Enemy>(50); // All enemnies in this test suite start with 50 HP.
                    
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }


};

class SlingshotTest : public testing::Test {
public:
    std::unique_ptr<Slingshot> slingshot;
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    SlingshotTest() {
        // You can do set-up work for each test here.

    }

    ~SlingshotTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
        slingshot = std::make_unique<Slingshot>(50, "red"); // All enemnies in this test suite start with 50 HP.

    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }


};

//A single test, not a fixture. No setup is called.
TEST(Enemy, First_test) {
    Enemy e(100);
    EXPECT_GT(e.getHealth(), 100);
    SUCCEED() << "Test test passed";
    FAIL() << "Test didn't pass";
}

// checks for if enemy has taken damage and popped.
TEST_F(EnemyTest, LethalDamagePopsPig) {
    enemy->takeDamage(60);
    EXPECT_TRUE(enemy->checkIfPopped());
}
// checks if the enemy's health is equal to 0.
TEST(Enemy, Second_Test) {
    Enemy e(100);
    ASSERT_EQ(e.getHealth(), 0);
}

// checks if the slingshot tension is > 50.
TEST(Slingshot, Third_Test) {
    Slingshot s(50, "red");
    EXPECT_LT(s.getTension(), 100);
} 

// checks if the bird type is "red".
TEST_F(SlingshotTest, Fourth_Test) {
    slingshot->loadBird("red");
    std::string str = "red";
    const char* c = str.c_str();
    ASSERT_EQ(slingshot->getBirdType(), c);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#include <Account.h>
#include <Transaction.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


class MockAccount : public Account {
public:
  using Account::Account;
  MOCK_METHOD(void,Lock, ());
  MOCK_METHOD(void, Unlock, ());
  MOCK_METHOD(void, ChangeBalance, (int amount));
  MOCK_METHOD(int, GetBalance, (), (const));
};


class TransactionTest : public ::testing::Test {
protected:

  MockAccount Alice{0,0}, Bob{1,0};
  Transaction test_tran;

  
  void SetUp() override {
    Alice.ChangeBalance((int)5000);
    Bob.ChangeBalance((int)(5000));
  }
};


TEST_F(TransactionTest, DefaultConstructorAndSettersAndGetters) {
  EXPECT_EQ(test_tran.fee(), 1);
  test_tran.set_fee(100);
  EXPECT_EQ(test_tran.fee(), 100);
}

TEST_F(TransactionTest, TransactionFails) {
  EXPECT_THROW(test_tran.Make(Alice, Alice, 1000), std::logic_error);
  EXPECT_THROW(test_tran.Make(Alice, Bob, -50), std::invalid_argument);
  EXPECT_THROW(test_tran.Make(Alice, Bob, 50), std::logic_error);
  if (test_tran.fee()*2-1 >= 100)
    EXPECT_FALSE(test_tran.Make(Alice, Bob, test_tran.fee()*2-1));
}


TEST_F(TransactionTest, SuccessfulTransaction) {
  EXPECT_FALSE(test_tran.Make(Alice, Bob, 1000));
  EXPECT_EQ(Bob.GetBalance(), 0);
  EXPECT_EQ(Alice.GetBalance(), 0);
}


TEST_F(TransactionTest, FailedTransactionNotEnoughMoney) {
  EXPECT_CALL(Alice, Lock()).Times(1);
  EXPECT_FALSE(test_tran.Make(Alice, Bob, 3900));
  EXPECT_EQ(Bob.GetBalance(), 0);
  EXPECT_EQ(Alice.GetBalance(), 0);
}

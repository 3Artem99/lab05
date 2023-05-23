#include <Account.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockAccount : public Account {
public:

  using Account::Account;
  MOCK_METHOD(int,GetBalance, (), (const));
  MOCK_METHOD(void, ChangeBalance, (int diff));
  MOCK_METHOD(void,Lock, ());
  MOCK_METHOD(void,Unlock, ());
};

MockAccount account(0,0);

TEST(Account, Banking) {
  EXPECT_CALL(account, GetBalance())
    .WillOnce(testing::Return(0));
    EXPECT_CALL(account, Lock());
  EXPECT_CALL(account, ChangeBalance(100))
    .WillOnce(testing::Throw(std::runtime_error("Account is locked")));
  
  EXPECT_CALL(account, Unlock());
  EXPECT_CALL(account, ChangeBalance(100))
    .WillOnce(testing::Return());
  
}

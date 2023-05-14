#include <Account.h> 
#include <gtest/gtest.h> 
#include <gmock/gmock.h> 

using namespace testing;

class MockAccount : public Account {
 public:
  MOCK_METHOD0(GetBalance, int());
  MOCK_METHOD1(ChangeBalance, void(int));
  MOCK_METHOD0(Lock, void());
  MOCK_METHOD0(Unlock, void());
};

TEST(Account, Banking){
  MockAccount test;
  EXPECT_CALL(test, GetBalance())
      .WillOnce(Return(0));
  EXPECT_CALL(test, ChangeBalance(100))
      .WillOnce(Throw(std::runtime_error("Account is locked")));
  EXPECT_CALL(test, Lock());
  EXPECT_CALL(test, ChangeBalance(100));
  EXPECT_CALL(test, GetBalance())
      .WillOnce(Return(100));
  EXPECT_CALL(test, Lock())
      .WillOnce(Throw(std::runtime_error("Account is already locked")));
  EXPECT_CALL(test, Unlock());
  EXPECT_CALL(test, ChangeBalance(100))
      .WillOnce(Throw(std::runtime_error("Account is locked")));

  test.Lock();
  ASSERT_THROW(test.ChangeBalance(100), std::runtime_error);
  test.Unlock();
  ASSERT_THROW(test.ChangeBalance(100), std::runtime_error);
}

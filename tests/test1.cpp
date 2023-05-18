#include <Account.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockAccount : public Account {
public:
MOCK_METHOD(int, GetBalance, ());
MOCK_METHOD(void, ChangeBalance, (int));
MOCK_METHOD(void, Lock, ());
MOCK_METHOD(void, Unlock, ());
};

MockAccount mock_account;

TEST(Account, Banking){
// Настраиваем Mock объект
EXPECT_CALL(mock_account, GetBalance())
.WillOnce(Return(0));
EXPECT_CALL(mock_account, ChangeBalance(100))
.WillOnce(Throw(std::runtime_error("Account is locked")));
EXPECT_CALL(mock_account, Lock());
EXPECT_CALL(mock_account, Unlock());
EXPECT_CALL(mock_account, ChangeBalance(100))
.WillOnce(Return());

// Используем Mock объект вместо реального объекта
Account* account = &mock_account;

ASSERT_EQ(account->GetBalance(), 0);
ASSERT_THROW(account->ChangeBalance(100), std::runtime_error);
account->Lock();
ASSERT_NO_THROW(account->ChangeBalance(100));
ASSERT_EQ(account->GetBalance(), 100);
ASSERT_THROW(account->Lock(), std::runtime_error);
account->Unlock();
ASSERT_THROW(account->ChangeBalance(100), std::runtime_error);
}

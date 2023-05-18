#include <Account.h>
#include <Transaction.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// создаем mock-объект класса Account
class MockAccount : public Account {
public:
  MOCK_METHOD(void, Lock, ());
  MOCK_METHOD(void, Unlock, ());
  MOCK_METHOD(bool, ChangeBalance, (int));
  MOCK_METHOD(int, GetBalance, (), (const));
};

// создаем тестовый класс
class TransactionTest : public ::testing::Test {
protected:
  // создаем mock-объекты и тестируемый объект
  MockAccount Alice, Bob;
  Transaction test_tran;

  // инициализируем mock-объекты
  void SetUp() override {
    Alice.SetBalance(5000);
    Bob.SetBalance(5000);
  }
};

// тестирование конструктора по умолчанию и сеттеров-геттеров
TEST_F(TransactionTest, DefaultConstructorAndSettersAndGetters) {
  EXPECT_EQ(test_tran.fee(), 1);
  test_tran.set_fee(100);
  EXPECT_EQ(test_tran.fee(), 100);
}

// тестирование случаев, когда транзакция не проходит
TEST_F(TransactionTest, TransactionFails) {
  EXPECT_THROW(test_tran.Make(Alice, Alice, 1000), std::logic_error);
  EXPECT_THROW(test_tran.Make(Alice, Bob, -50), std::invalid_argument);
  EXPECT_THROW(test_tran.Make(Alice, Bob, 50), std::logic_error);
  if (test_tran.fee()*2-1 >= 100)
    EXPECT_FALSE(test_tran.Make(Alice, Bob, test_tran.fee()*2-1));
}

// тестирование лоча и анлоча
TEST_F(TransactionTest, LockAndUnlock) {
  EXPECT_CALL(Alice, Lock()).Times(1);
  EXPECT_THROW(test_tran.Make(Alice, Bob, 1000), std::runtime_error);
  EXPECT_CALL(Alice, Unlock()).Times(1);
}

// тестирование успешной транзакции
TEST_F(TransactionTest, SuccessfulTransaction) {
  EXPECT_CALL(Alice, Lock()).Times(1);
  EXPECT_CALL(Bob, ChangeBalance(1000)).WillOnce(::testing::Return(true));
  EXPECT_CALL(Alice, ChangeBalance(-1100)).WillOnce(::testing::Return(true));
  EXPECT_CALL(Alice, Unlock()).Times(1);
  EXPECT_TRUE(test_tran.Make(Alice, Bob, 1000));
  EXPECT_EQ(Bob.GetBalance(), 6000);
  EXPECT_EQ(Alice.GetBalance(), 3900);
}

// тестирование неуспешной транзакции из-за нехватки средств
TEST_F(TransactionTest, FailedTransactionNotEnoughMoney) {
  EXPECT_CALL(Alice, Lock()).Times(1);
  EXPECT_FALSE(test_tran.Make(Alice, Bob, 3900));
  EXPECT_EQ(Bob.GetBalance(), 5000);
  EXPECT_EQ(Alice.GetBalance(), 5000);
}

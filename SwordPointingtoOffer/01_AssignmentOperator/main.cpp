// 面试题1：赋值运算符函数
// 题目：如下为类型CMyString的声明，请为该类型添加赋值运算符函数。

/* 
 * 当定义一个赋值运算符函数时，需注意：
 * 1. 应将返回值的类型声明为该类型的引用，并在函数结束前返回实例自身的引用(*this)。只有返回一个引用，才可以运行连续赋值(a = b = c)
 * 2. 应将传入的参数的类型声明为常量引用
 * 3. 及时释放实例自身已有的内存，否则会出现内存泄漏
 * 4. 需判断传入的参数和当前的示例是否是同一个示例。若事先不判断就进行赋值，那么在释放实例自身内存的时候就会导致严重的问题。
 */

#include <cstdio>
#include <cstring>

class CMyString {
public:
  CMyString(char* pData = nullptr);
  CMyString(const CMyString& str);
  ~CMyString(void);

  CMyString& operator=(const CMyString& str);

  void Print();

private:
  char* m_pData;
};

CMyString::CMyString(char* pData) {
  if (pData == nullptr) {
    m_pData = new char[1];
    m_pData[0] = '\0';
  } else {
    int length = strlen(pData);
    m_pData = new char[length + 1];
    strcpy(m_pData, pData);
  }
}
CMyString::CMyString(const CMyString& str) {
  int length = strlen(str.m_pData);
  m_pData = new char[length + 1];
  strcpy(m_pData, str.m_pData);
}
CMyString::~CMyString() {
  delete[] m_pData;
}
#if 0
// 若内存不足导致 new char 抛出异常，则 m_pData 将是一个空指针，容易导致程序崩溃，违背了异常安全性原则
CMyString& CMyString::operator=(const CMyString& str) {
  if (this == &str) {
    return *this;
  }
  delete[] m_pData;
  m_pData = nullptr;

  m_pData = new char[strlen(str.m_pData) + 1];
  strcpy(m_pData, str.m_pData);
  return *this;
}
#endif

// 先创建一个临时的实例 tmp_str，接着把临时实例的 m_pData 和实例自身的 m_pData 进行交换。
// 这样，当局部变量 tmp_str 被析构时，它指向的原本实例的 m_pData 的内存就被释放掉了
// 在这种方法中，若由于内存不足抛出 bad alloc 等异常，因为我们并没有修改原来实例的状态，故
// 实例的状态还是有效的，这也就保证了异常安全性
CMyString& CMyString::operator=(const CMyString& str) {
  if (this != &str) {
    CMyString tmp_str(str);

    char* tmp_data = tmp_str.m_pData;
    tmp_str.m_pData = m_pData;
    m_pData = tmp_data;
  }
  return *this;
}

void CMyString::Print() {
  printf("%s\n", m_pData);
}

//  ==================== testing ======================

void Test1() {
  printf("Test1 begins:\n");

  char* text = "Hello world";

  CMyString str1(text);
  CMyString str2;
  str2 = str1;

  printf("The expected result is: %s.\n", text);

  printf("The actual result is: ");
  str2.Print();
  printf(".\n");
}

// 赋值给自己
void Test2() {
  printf("Test2 begins:\n");

  char* text = "Hello world";

  CMyString str1(text);
  str1 = str1;

  printf("The expected result is: %s.\n", text);

  printf("The actual result is: ");
  str1.Print();
  printf(".\n");
}

// 连续赋值
void Test3() {
  printf("Test3 begins:\n");

  char* text = "Hello world";

  CMyString str1(text);
  CMyString str2, str3;
  str3 = str2 = str1;

  printf("The expected result is: %s.\n", text);

  printf("The actual result is: ");
  str2.Print();
  printf(".\n");

  printf("The expected result is: %s.\n", text);

  printf("The actual result is: ");
  str3.Print();
  printf(".\n");
}

int main(int argc, char* argv[]) {
  Test1();
  Test2();
  Test3();

  return 0;
}
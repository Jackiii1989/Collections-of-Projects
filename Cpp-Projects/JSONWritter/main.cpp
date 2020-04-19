#include <iostream>
#include <string>
#include "JSONWriter.h"

using namespace std;
using namespace JSON;

class Person : public Object {
public:

  Person(string name, int age) : mName(move(name)), mAge(age) {}

  virtual void writeJson(ostream &out) const override {
    writeObject(out, "name", mName, "age", mAge);
  }

private:
  string mName;
  int mAge;
};

template <typename... TArgs>
void writeArrayLn(TArgs &&...args)
{
  writeArray(cout, std::forward<TArgs>(args)...);
  cout << endl;
}

template <typename... TArgs>
void writeObjectLn(TArgs &&...args)
{
  writeObject(cout, std::forward<TArgs>(args)...);
  cout << endl;
}

int main()
{

  writeObjectLn("x", 1, "y", 2); // {"x": 1, "y": 2 }
  
  Person joe("Joe", 40);
  joe.writeJson(cout);   // {"name": "Joe", "age": 40 }
  cout << endl;
  
  Person jack("Jack", 30);
  writeArrayLn(joe, jack);       // [{"name": "Joe", "age": 40}, {"name": "Jack", "age": 30}]
  
  Person *nobody = nullptr;
  writeArrayLn(nobody);
  
  writeArrayLn(&joe, &jack, nobody); // [{"name": "Joe", "age": 40}, {"name": "Jack", "age": 30}, null]
  
  cout << endl;
  return 0;
}


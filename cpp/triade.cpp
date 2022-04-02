#include <iostream>
#include <string>
#include <cstdio>

class Triad {
protected:
  int first;
  int second;
  int third;

public:
  Triad(): first(0), second(0), third(0) {}

  Triad(int first, int second, int third)
  : first(first), second(second), third(third) 
  {
  }
  
  const int& get_first_num() const {
    return first;
  }

  const int& get_second_num() const {
    return second;
  }

  const int& get_third_num() const {
    return third;
  }

  void set_first_num(const int& number) {
    this->first = number;
  }

  void set_second_num(const int& number) {
    this->second = number;
  }
  
  void set_third_num(const int& number) {
    this->third = number;
  }

  friend bool operator== (const Triad& obj1, const Triad& obj2);
  friend std::ostream& operator<< (std::ostream& out, const Triad& obj);
};

bool operator== (const Triad& obj1, const Triad& obj2) {
  return ((obj1.first == obj2.first) && 
          (obj1.second == obj2.second) && 
          (obj1.third == obj2.third));
}

bool operator!= (const Triad& obj1, const Triad& obj2) {
  return !(obj1==obj2);
}

std::ostream& operator<< (std::ostream& out, const Triad& obj) {
  out << obj.first << ' ' << obj.second << ' ' << obj.third;

  return out;
}


class Data: public Triad {
public:
  // First field - day, second field - month, third field - year
  // Access to fields match Acess to field in Base class
  Data(const int& day, const int& month, const int& year) {
    if (year >= 0 && year <= 9999) {
      this->third = year;
    }
    else {
      std::cout << "Bad input of year\n";
      exit(0);
    }

    int day_in_month;

    switch (month) {
      case 1:day_in_month = 31; break;
      case 2:{
                if (year % 4 == 0) {
                  day_in_month = 28;
                } else {
                  day_in_month = 29;
                }
                break;
              }
      case 3: day_in_month = 31; break;
      case 4: day_in_month = 30; break;
      case 5: day_in_month = 31; break;
      case 6: day_in_month = 30; break;
      case 7: day_in_month = 31; break;
      case 8: day_in_month = 31; break;
      case 9: day_in_month = 30; break;
      case 10: day_in_month = 31; break;
      case 11: day_in_month = 30; break;
      case 12: day_in_month = 31; break;
      default: std::cout << "Bad input of month\n"; exit(0);
    }

    this->second = month;

    if (day >= 0 && day <= day_in_month) {
      this->first = day;
    } 
    else {
      std::cout << "Bad input of day\n";
      exit(0); 
    }
  }

  Triad* base_ptr() {
    return static_cast<Triad*>(this);
  }

  void to_base(const Triad& obj) {
    *this = Data({obj.get_first_num(), obj.get_second_num(), obj.get_third_num()});
  }

  bool operator== (const Data& obj) {
    return ((this->first == obj.first) && 
            (this->second == obj.second) && 
            (this->third == obj.third));
  }

  bool operator!= (const Data& obj) {
    return !(*this == obj);
  }

  bool operator> (const Data& obj) {
    int d1 = this->first + this->second*31 + this->third*365;
    int d2 = obj.first + obj.second*31 + obj.third*365;
    
    return d1 > d2; 
  }

  bool operator< (const Data& obj) {
    return (!(*this > obj) && (*this != obj));
  }

  bool operator>= (const Data& obj) {
    return !(*this < obj);
  }

  bool operator<= (const Data& obj) {
    return !(*this > obj);
  }

  const char* to_string() const {
    char* str = new char[11];

    sprintf(str, "%02d.%02d.%04d", first, second, third);

    return str;
  }

  friend std::ostream& operator<< (std::ostream& out, const Data& obj);
};

std::ostream& operator<< (std::ostream& out, const Data& obj) {
  out << obj.first << ' ' << obj.second << ' ' << obj.third;

  return out;
}


int main() {
  
  Triad obj1 = {1, 2, 3};
  Triad obj2 = obj1;
  Triad* obj3 = new Triad;
  Triad* obj_mas = new Triad[10];
  Triad obj_mas_2[10];

  obj1.get_first_num();
  obj1.get_second_num();
  obj1.get_third_num();

  obj1.set_first_num(10);
  obj1.set_second_num(10);
  obj1.set_third_num(10);

  std::cout << (Triad({1, 2, 3}) == obj1) << '\n' << (Triad({1, 2, 3}) != obj1) << std::endl;

  Data obj4 = {1, 2, 3};
  Data obj5 = {29, 2, 3};

  obj4.get_first_num();
  obj4.get_second_num();
  obj4.get_third_num();

  std::cout << obj4.to_string() << std::endl;

  std::cout << (Data({1, 2, 3}) == obj4) << '\n' << (Data({1, 2, 3}) != obj5) << std::endl;
  std::cout << (Data({1, 2, 3}) >= obj4) << '\n' << (Data({1, 2, 3}) <= obj5) << std::endl;
  std::cout << (Data({1, 2, 3}) > obj4) << '\n' << (Data({1, 2, 3}) < obj5) << std::endl;

  Triad* current_obj = obj4.base_ptr();

  std::cout << *current_obj << std::endl;

  obj4.to_base(obj1);

  std::cout << *current_obj << std::endl;

  return 0;
}
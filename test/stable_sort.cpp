 ///
 /// @file    stable_sort.cpp
 /// @author  kkmjy(mjy332528@163.com)
 /// @date    2017-09-06 10:02:57
 ///
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
 
struct Employee 
{
    Employee(int age, std::string name)
	: age(age)
	, name(name) 
	{ } 
    int age;
    std::string name;  // Does not participate in comparisons
};

struct Compare
{
	bool operator()(const Employee &lhs, const Employee &rhs) 
	{
		if(lhs.age > rhs.age)
			return true;
		else
			return false;
	}
}cmp;

int main()
{
    std::vector<Employee> v = { 
        Employee(108, "Zaphod"),
        Employee(32, "Arthur"),
        Employee(108, "Ford"),
    };  
 
    std::stable_sort(v.begin(), v.end(),cmp);
    for (const Employee &e : v) {
        std::cout << e.age << ", " << e.name << '\n';
    }   
} 
